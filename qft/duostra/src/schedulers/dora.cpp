#include "qft_scheduler.hpp"

#include <omp.h>
#include <algorithm>
#include <functional>
#include <unordered_set>
#include <vector>
#include "util.hpp"

using namespace std;
using namespace scheduler;

TreeNode::TreeNode(TreeNodeConf conf,
                   size_t gate_idx,
                   unique_ptr<QFTRouter> router,
                   unique_ptr<Base> scheduler)
    : TreeNode(conf,
               move(vector<size_t>({gate_idx})),
               move(router),
               move(scheduler)) {}

TreeNode::TreeNode(TreeNodeConf conf,
                   vector<size_t>&& gate_indices,
                   unique_ptr<QFTRouter> router,
                   unique_ptr<Base> scheduler)
    : conf_(conf),
      gate_indices_(move(gate_indices)),
      children_({}),
      router_(move(router)),
      scheduler_(move(scheduler)) {
    route_internal_gates();
}

TreeNode::TreeNode(const TreeNode& other)
    : conf_(other.conf_),
      gate_indices_(other.gate_indices_),
      children_(other.children_),
      router_(other.router_->clone()),
      scheduler_(other.scheduler_->clone()) {}

TreeNode::TreeNode(TreeNode&& other)
    : conf_(other.conf_),
      gate_indices_(move(other.gate_indices_)),
      children_(move(other.children_)),
      router_(move(other.router_)),
      scheduler_(move(other.scheduler_)) {}

TreeNode& TreeNode::operator=(const TreeNode& other) {
    conf_ = other.conf_;
    gate_indices_ = other.gate_indices_;
    children_ = other.children_;
    router_ = other.router_->clone();
    scheduler_ = other.scheduler_->clone();
    return *this;
}

TreeNode& TreeNode::operator=(TreeNode&& other) {
    conf_ = other.conf_;
    gate_indices_ = move(other.gate_indices_);
    children_ = move(other.children_);
    router_ = move(other.router_);
    scheduler_ = move(other.scheduler_);
    return *this;
}

vector<TreeNode>& TreeNode::children() {
    grow_if_needed();
    return children_;
}

size_t TreeNode::immediate_next() const {
    size_t gate_idx = scheduler().get_executable(*router_);
    const auto& avail_gates = scheduler().get_avail_gates();

    if (gate_idx != ERROR_CODE) {
        return gate_idx;
    }

    if (avail_gates.size() == 1) {
        return avail_gates[0];
    }

    return ERROR_CODE;
}

void TreeNode::route_internal_gates() {
    assert(children_.empty());

    // Execute the initial gates.
    for (size_t gate_idx : gate_indices_) {
        [[maybe_unused]] const auto& avail_gates =
            scheduler().get_avail_gates();

        assert(std::find(avail_gates.begin(), avail_gates.end(), gate_idx) !=
               avail_gates.end());

        scheduler_->route_one_gate(*router_, gate_idx);

        assert(std::find(avail_gates.begin(), avail_gates.end(), gate_idx) ==
               avail_gates.end());
    }

    // Execute additional gates if exec_single.
    if (gate_indices_.empty() || !conf_.exec_single) {
        return;
    }

    size_t gate_idx;
    while ((gate_idx = immediate_next()) != ERROR_CODE) {
        scheduler_->route_one_gate(*router_, gate_idx);
        gate_indices_.push_back(gate_idx);
    }

    unordered_set<size_t> executed{gate_indices_.begin(), gate_indices_.end()};
    assert(executed.size() == gate_indices_.size());
}

template <>
inline void std::swap<TreeNode>(TreeNode& a, TreeNode& b) {
    TreeNode c{std::move(a)};
    a = std::move(b);
    b = std::move(c);
}

// Cost recursively calls children's cost, and selects the best one.
size_t TreeNode::best_cost(int depth) {
    // Grow if remaining depth >= 2.
    // Terminates on leaf nodes.
    if (depth > 0) {
        grow_if_needed();
    }

    if (depth <= 0 || is_leaf()) {
        return scheduler().ops_cost();
    }

    // Calls the more efficient best_cost() when depth is only 1.
    if (depth == 1) {
        return best_cost();
    }

    assert(children_.size() != 0);

    size_t best = (size_t)-1;

    sort(children_.begin(), children_.end(),
         [](const TreeNode& a, const TreeNode& b) {
             return a.scheduler().ops_cost() < b.scheduler().ops_cost();
         });

    auto end = conf_.candidates < children_.size()
                   ? children_.begin() + conf_.candidates
                   : children_.end();

// Calcualtes the best cost for each children.
#pragma omp parallel for
    for (auto child = children_.begin(); child < end; ++child) {
        size_t cost = child->best_cost(depth - 1);

#pragma omp critical
        if (cost < best) {
            best = cost;
        }
    }

    // Clear the cache if specified.
    if (conf_.never_cache) {
        children_.clear();
    }

    return best;
}

size_t TreeNode::best_cost() const {
    size_t best = (size_t)-1;

    const auto& avail_gates = scheduler().get_avail_gates();

#pragma omp parallel for
    for (size_t idx = 0; idx < avail_gates.size(); ++idx) {
        TreeNode child_node{conf_, avail_gates[idx], router().clone(),
                            scheduler().clone()};
        size_t cost = child_node.scheduler().ops_cost();

#pragma omp critical
        if (cost < best) {
            best = cost;
        }
    }

    return best;
}

// Grow by adding availalble gates to children.
void TreeNode::grow() {
    const auto& avail_gates = scheduler().get_avail_gates();

    assert(children_.empty());
    children_.reserve(avail_gates.size());

    for (size_t gate_idx : avail_gates) {
        children_.emplace_back(conf_, gate_idx, router().clone(),
                               scheduler().clone());
    }
}

inline void TreeNode::grow_if_needed() {
    if (is_leaf()) {
        grow();
    }
}

TreeNode TreeNode::best_child(int depth) {
    auto& next_nodes = children();
    size_t best_idx = 0, best = (size_t)-1;

#pragma omp parallel for
    for (size_t idx = 0; idx < next_nodes.size(); ++idx) {
        auto& node = next_nodes[idx];

        assert(depth >= 1);
        size_t cost = node.best_cost(depth);

#pragma omp critical
        if (cost < best) {
            best_idx = idx;
            best = cost;
        }
    }

    return next_nodes[best_idx];
}

Dora::Dora(unique_ptr<Topology> topo, const json& conf)
    : Greedy(move(topo), conf),
      look_ahead(json_get<int>(conf, "depth")),
      never_cache_(json_get<bool>(conf, "never_cache")),
      exec_single_(json_get<bool>(conf, "exec_single")) {
    cache_only_when_necessary();
}

Dora::Dora(const Dora& other)
    : Greedy(other),
      look_ahead(other.look_ahead),
      never_cache_(other.never_cache_),
      exec_single_(other.exec_single_) {}

Dora::Dora(Dora&& other)
    : Greedy(other),
      look_ahead(other.look_ahead),
      never_cache_(other.never_cache_),
      exec_single_(other.exec_single_) {}

unique_ptr<Base> Dora::clone() const {
    return make_unique<Dora>(*this);
}

void Dora::cache_only_when_necessary() {
    if (!never_cache_ && look_ahead == 1) {
        cerr << "When look_ahead = 1, 'never_cache' is used by default.\n";
        never_cache_ = true;
    }
}

void Dora::assign_gates(unique_ptr<QFTRouter> router) {
    auto total_gates = topo_->get_num_gates();

    auto root = make_unique<TreeNode>(
        TreeNodeConf{never_cache_, exec_single_, conf_.candidates},
        move(vector<size_t>()), router->clone(), clone());

    // For each step. (all nodes + 1 dummy)
    TqdmWrapper bar{total_gates + 1};
    do {
        // Update the candidates.
        auto selected_node =
            make_unique<TreeNode>(root->best_child(look_ahead));

        root = move(selected_node);

        for (size_t gate_idx : root->executed_gates()) {
            route_one_gate(*router, gate_idx);
            ++bar;
        }
    } while (!root->done());
}
