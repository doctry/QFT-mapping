#include "qft_scheduler.hpp"

#include <omp.h>
#include <algorithm>
#include <functional>
#include <vector>

using namespace std;
using namespace scheduler;

TreeNode::TreeNode(size_t gate_idx,
                   unique_ptr<QFTRouter> router,
                   unique_ptr<SchedulerBase> scheduler)
    : gate_idx_(gate_idx), router_(move(router)), scheduler_(move(scheduler)) {
    exec_route();
}

TreeNode::TreeNode(TreeNode&& other)
    : gate_idx_(other.gate_idx_),
      children_(move(other.children_)),
      router_(move(other.router_)),
      scheduler_(move(other.scheduler_)) {}

TreeNode& TreeNode::operator=(TreeNode&& other) {
    gate_idx_ = other.gate_idx_;
    children_ = move(other.children_);
    router_ = move(other.router_);
    scheduler_ = move(other.scheduler_);
    return *this;
}

void TreeNode::exec_route() {
    assert(children_.empty());

    [[maybe_unused]] const auto& gates = scheduler_->get_avail_gates();

    assert(std::find(gates.begin(), gates.end(), gate_idx_) != gates.end());

    scheduler_->route_one_gate(*router_, gate_idx_);

    assert(std::find(gates.begin(), gates.end(), gate_idx_) == gates.end());

    for (size_t gate_idx = scheduler_->get_executable(*router_);
         gate_idx != size_t(-1);
         gate_idx = scheduler_->get_executable(*router_)) {
        scheduler_->route_one_gate(*router_, gate_idx);
    }
}

// Size calcuates the tree size.
size_t TreeNode::num_leafs(int depth) const {
    auto size_fn = [](const TreeNode&) -> size_t { return 1; };

    auto sum = [](const vector<size_t>& sizes) -> size_t {
        return accumulate(sizes.begin(), sizes.end(), 0);
    };

    return recursive<size_t>(depth, size_fn, sum);
}

// Cost recursively calls children's cost, and selects the best one.
size_t TreeNode::best_cost(int depth) const {
    auto cost_fn = [](const TreeNode& node) -> size_t {
        return node.scheduler_->ops_cost();
    };

    auto select_best = [](const vector<size_t>& costs) -> size_t {
        return *min_element(costs.begin(), costs.end());
    };

    return recursive<size_t>(depth, cost_fn, select_best);
}

vector<reference_wrapper<TreeNode>> TreeNode::leafs(int depth) {
    using vec_nodes = vector<reference_wrapper<TreeNode>>;
    auto as_reference = [](TreeNode& node) -> vec_nodes { return {ref(node)}; };

    auto collect_all = [](const vector<vec_nodes>& vec_of_leafs) -> vec_nodes {
        size_t total_sizes =
            accumulate(vec_of_leafs.begin(), vec_of_leafs.end(), 0,
                       [](size_t total_so_far, const vec_nodes& second) {
                           return total_so_far + second.size();
                       });

        vec_nodes result;
        result.reserve(total_sizes);

        for (const auto& leafs : vec_of_leafs) {
            result.insert(result.end(), leafs.begin(), leafs.end());
        }

        return result;
    };

    return recursive<vec_nodes>(depth, as_reference, collect_all);
}

template <typename T>
T TreeNode::recursive(int depth,
                      function<T(const TreeNode&)> func,
                      function<T(const vector<T>&)> collect) const {
    auto wrapped_func = [func](TreeNode& node) -> T {
        // Since TreeNode isn't actually modified in the template
        // the casting is safe.
        const TreeNode& cnode = const_cast<const TreeNode&>(node);
        return func(cnode);
    };

    return const_cast<TreeNode*>(this)->recursive<T>(depth, wrapped_func,
                                                     collect);
}

template <typename T>
T TreeNode::recursive(int depth,
                      function<T(TreeNode&)> func,
                      function<T(const vector<T>&)> collect) {
    // Terminates on leaf nodes.
    if (depth <= 0 || is_leaf()) {
        return func(*this);
    }

    // Apply transformations recursively.
    // https://stackoverflow.com/questions/27144054/why-is-the-stdinitializer-list-constructor-preferred-when-using-a-braced-initi
    // Initializer should never be used here as it gets confused with a list of
    // 2 elements.
    vector<T> transforms;
    transforms.reserve(children_.size());

    // Transform from TreeNode to collected data from leafs of each TreeNode.
    transform(children_.begin(), children_.end(), back_inserter(transforms),
              [depth, func, collect](unique_ptr<TreeNode>& child) {
                  return child->recursive<T>(depth - 1, func, collect);
              });

    // Collect tranformations.
    return collect(transforms);
}

// Grow by adding availalble gates to children.
void TreeNode::grow() {
    assert(children_.empty());
    const auto& avail_gates = scheduler_->get_avail_gates();
    children_.reserve(avail_gates.size());
    for (size_t gate_idx : avail_gates) {
        auto ptr = make_unique<TreeNode>(gate_idx, router().clone(),
                                         scheduler().clone());
        children_.push_back(move(ptr));
    }
}

inline void TreeNode::grow_if_needed() {
    if (is_leaf()) {
        grow();
    }
}

Dora::Dora(unique_ptr<Topology> topo, const json& conf)
    : Greedy(move(topo), conf), look_ahead(json_get<int>(conf, "depth")) {}

Dora::Dora(const Dora& other) : Greedy(other), look_ahead(other.look_ahead) {}

Dora::Dora(Dora&& other) : Greedy(other), look_ahead(other.look_ahead) {}

unique_ptr<SchedulerBase> Dora::clone() const {
    return make_unique<Dora>(*this);
}

void Dora::assign_gates(unique_ptr<QFTRouter> router) {
    auto total_gates = topo_->get_num_gates();

    Tqdm bar{total_gates};
    vector<unique_ptr<TreeNode>> next_trees;

    // For each step.
    for (size_t idx = 0; idx < total_gates; ++idx, bar.add()) {
        auto avail_gates = topo_->get_avail_gates();

        // Generate heuristic trees if not present.
        // Since router and this both outlive update_next_trees,
        // this usage is safe.
        update_next_trees(*router, *this, avail_gates, next_trees);

        // Calcuate each tree's costs and find the best one (smallest cost).
        vector<size_t> costs;
        transform(next_trees.begin(), next_trees.end(), back_inserter(costs),
                  [this](const unique_ptr<TreeNode>& root) {
                      return root->best_cost(look_ahead);
                  });

#ifdef DEBUG
        cout << "1\n";
        cout << "costs sizes and tree sizes: " << costs.size() << " "
             << next_trees.size() << "\n";
#endif

        assert(costs.size() == next_trees.size());
        auto min = min_element(costs.begin(), costs.end());
        size_t argmin = min - costs.begin();
        assert(costs.size() != 0);

#ifdef DEBUG
        cout << "2\n";
        cout << "Argmin: " << argmin << "\n";
        cout << "Costs: " << costs << "\n";
#endif

        // Update the candidates.
        size_t gate_idx = avail_gates.at(argmin);
        auto selected_tree{move(next_trees.at(argmin))};
        next_trees = move(selected_tree->children());

        route_one_gate(*router, gate_idx);
    }
}

// Check if ids are matched with chidren's ids.
static void children_match_ids(const vector<size_t>& ids,
                               const vector<unique_ptr<TreeNode>>& children) {
    unordered_set<size_t> all_children{ids.begin(), ids.end()};

    assert(all_children.size() == children.size());
    for ([[maybe_unused]] const auto& child : children) {
        assert(all_children.find(child->gate_idx()) != all_children.end());
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
                             vector<unique_ptr<TreeNode>>& next_trees) const {
    if (next_trees.empty()) {
        for (size_t idx : next_ids) {
            auto ptr =
                make_unique<TreeNode>(idx, router.clone(), scheduler.clone());
            next_trees.push_back(move(ptr));
        }
    }

    assert(next_trees.size() == next_ids.size());
    for (auto& tree : next_trees) {
        update_tree_recursive(look_ahead, *tree);
    }
}

// Update and grow the trees given by the root recursively.
// If `remaining_depth` is reached but there are still available gates,
// the tree is extended.
void Dora::update_tree_recursive(int remaining_depth, TreeNode& root) const {
    const auto& avail_gates = root.scheduler().get_avail_gates();

    if (remaining_depth <= 0 || avail_gates.empty()) {
        return;
    }

    // If the heuristic tree has reached the leaf, extend it.
    root.grow_if_needed();

    // Update children heuristic search tree.
    root_match_avail_gates(root.scheduler(), root);

    bool use_omp = getenv("OMP_NUM_THREADS") != NULL;
    for (auto& child_node : root.children()) {
        if (use_omp) {
            update_tree_recursive_parallel(remaining_depth - 1, *child_node);
        } else {
            update_tree_recursive(remaining_depth - 1, *child_node);
        }
    }
}

void Dora::update_tree_recursive_parallel(int total_depth,
                                          TreeNode& root) const {
    const size_t threads = static_cast<size_t>(omp_get_thread_num());

    // Using update_tree_recursive to grow the tree.
    // Since calling update_tree_recursive on nodes that have children doesn't
    // modify the node, it should be ok.
    int depth;
    for (depth = 0;
         update_tree_recursive(depth, root), root.num_leafs(depth) < threads;
         ++depth)
        ;
    assert(root.num_leafs(depth) >= threads);

    vector<reference_wrapper<TreeNode>> leafs = root.leafs(depth);

    // Update sub trees in parallel.
#pragma omp parallel for
    for (size_t idx = 0; idx < leafs.size(); ++idx) {
        TreeNode& leaf = leafs[idx];
        update_tree_recursive(total_depth - depth, leaf);
    }
}
