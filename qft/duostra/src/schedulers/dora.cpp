#include "qft_scheduler.hpp"

#include <algorithm>
#include <vector>

using namespace std;
using namespace scheduler;

TreeNode::TreeNode(size_t gate_idx,
                   unique_ptr<QFTRouter> router,
                   unique_ptr<SchedulerBase> scheduler) noexcept
    : gate_idx_(gate_idx),
      children_({}),
      router_(move(router)),
      scheduler_(move(scheduler)) {}

TreeNode::TreeNode(const TreeNode& other) noexcept
    : gate_idx_(other.gate_idx_),
      children_(other.children_),
      router_(other.router_->clone()),
      scheduler_(other.scheduler_->clone()) {}

TreeNode& TreeNode::operator=(const TreeNode& other) noexcept {
    gate_idx_ = other.gate_idx_;
    children_ = other.children_;
    router_ = other.router_->clone();
    scheduler_ = other.scheduler_->clone();
    return *this;
}

// Cost recursively calls children's cost, and selects the best one.
size_t TreeNode::cost(int depth) const {
    // The leaf node contains the scheduling results.
    if (depth <= 0 || is_leaf()) {
        return scheduler_->ops_cost();
    }

    // Recursively calculates costs and returns the best cost.
    vector<size_t> costs{children_.size(), 0};
    transform(children_.begin(), children_.end(), costs.begin(),
              [depth](const TreeNode& child) { return child.cost(depth - 1); });
    return *min_element(costs.begin(), costs.end());
}

Dora::Dora(unique_ptr<Topology> topo, json& conf) noexcept
    : Greedy(move(topo), conf), depth(json_get<int>(conf, "depth")) {}

Dora::Dora(const Dora& other) noexcept : Greedy(other), depth(other.depth) {}

Dora::Dora(Dora&& other) noexcept : Greedy(other), depth(other.depth) {}

unique_ptr<SchedulerBase> Dora::clone() const {
    return make_unique<Dora>(*this);
}

void Dora::assign_gates(unique_ptr<QFTRouter> router) {
    auto total_gates = topo_->get_num_gates();

    Tqdm bar{int(total_gates)};
    vector<TreeNode> next_trees;

    // For each step.
    for (size_t idx = 0; idx < total_gates; ++idx, bar.add()) {
        auto avail_gates = topo_->get_avail_gates();

        // Generate heuristic trees if not present.
        // Since router and this both outlive update_next_trees,
        // this function is safe.
        update_next_trees(*router, *this, avail_gates, next_trees);

        // Calcuate each tree's costs and find the best one (smallest cost).
        vector<size_t> costs{next_trees.size(), 0};
        transform(next_trees.begin(), next_trees.end(), costs.begin(),
                  [this](const TreeNode& root) { return root.cost(depth); });
        auto argmin = min_element(costs.begin(), costs.end()) - costs.begin();

        size_t gate_idx = avail_gates[argmin];

        route_one_gate(*router, gate_idx);

        // Update the candidates.
        next_trees = next_trees[argmin].children();
    }
}

// Check if ids are matched with chidren's ids.
static void children_match_ids(const vector<size_t>& ids,
                               const vector<TreeNode>& children) {
    unordered_set<size_t> all_children{ids.begin(), ids.end()};

    assert(all_children.size() == children.size());
    for (const auto& child : children) {
        assert(all_children.find(child.gate_idx()) != all_children.end());
    }
}

// If a tree is already present, the children of the root nodes must match.
static void root_match_avail_gates(const SchedulerBase& scheduler,
                                   const TreeNode& root) {
    assert(!root.is_leaf());

    children_match_ids(scheduler.get_avail_gates(), root.children());
}

void Dora::update_next_trees(const QFTRouter& router,
                             const SchedulerBase& scheduler,
                             const vector<size_t>& next_ids,
                             vector<TreeNode>& next_trees) {
    if (next_trees.empty()) {
        for (size_t idx : next_ids) {
            next_trees.push_back(
                TreeNode{idx, router.clone(), scheduler.clone()});
        }
    }

    assert(next_trees.size() == next_ids.size());
    for (auto& tree : next_trees) {
        update_tree_recursive(depth, tree);
    }
}

// Update and grow the trees given by the root recursively.
// If `remaining_depth` is reached but there are still available gates,
// the tree is extended.
void Dora::update_tree_recursive(int remaining_depth, TreeNode& root) {
    const auto& avail_gates = root.scheduler().get_avail_gates();

    if (remaining_depth <= 0 || avail_gates.empty()) {
        return;
    }

    // If the heuristic tree has reached the leaf, extend it.
    if (root.is_leaf()) {
        for (size_t gate_idx : avail_gates) {
            auto cloned_router = root.router().clone();
            auto cloned_sched = root.scheduler().clone();

            cloned_sched->route_one_gate(*cloned_router, gate_idx);
            root.children().push_back(
                TreeNode{gate_idx, move(cloned_router), move(cloned_sched)});
        }
    }

    // Update children heuristic search tree.
    root_match_avail_gates(root.scheduler(), root);
    for (auto& child_node : root.children()) {
        update_tree_recursive(remaining_depth - 1, child_node);
    }
}
