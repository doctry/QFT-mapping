#include "qft_scheduler.hpp"

#include <algorithm>
#include <vector>

using namespace std;
using namespace scheduler;

TreeNode::TreeNode(size_t gate_idx,
                   unique_ptr<QFTRouter> router,
                   unique_ptr<SchedulerBase> scheduler)
    : gate_idx_(gate_idx),
      children_({}),
      router_(move(router)),
      scheduler_(move(scheduler)) {
    exec_route();
}

TreeNode::TreeNode(const TreeNode& other)
    : gate_idx_(other.gate_idx_),
      children_(other.children_),
      router_(other.router_->clone()),
      scheduler_(other.scheduler_->clone()) {}

TreeNode::TreeNode(TreeNode&& other)
    : gate_idx_(other.gate_idx_),
      children_(move(other.children_)),
      router_(move(other.router_)),
      scheduler_(move(other.scheduler_)) {}

TreeNode& TreeNode::operator=(const TreeNode& other) {
    gate_idx_ = other.gate_idx_;
    children_ = other.children_;
    router_ = other.router_->clone();
    scheduler_ = other.scheduler_->clone();
    return *this;
}

TreeNode& TreeNode::operator=(TreeNode&& other) {
    gate_idx_ = other.gate_idx_;
    children_ = move(other.children_);
    router_ = move(other.router_);
    scheduler_ = move(other.scheduler_);
    return *this;
}

void TreeNode::exec_route() {
    [[maybe_unused]] const auto& gates = scheduler_->get_avail_gates();

    assert(std::find(gates.begin(), gates.end(), gate_idx_) != gates.end());

    scheduler_->route_one_gate(*router_, gate_idx_);

    assert(std::find(gates.begin(), gates.end(), gate_idx_) == gates.end());
}

// Size calcuates the tree size.
size_t TreeNode::tree_size(int depth) const {
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

template <typename T>
T TreeNode::recursive(int depth,
                      T leaf_function(const TreeNode&),
                      T collect(const vector<T>&)) const {
    // Terminates on leaf nodes.
    if (depth <= 0 || is_leaf()) {
        return leaf_function(*this);
    }

    // Apply transformations recursively.
    // https://stackoverflow.com/questions/27144054/why-is-the-stdinitializer-list-constructor-preferred-when-using-a-braced-initi
    // Initializer should never be used here as it gets confused with a list of
    // 2 elements.
    vector<size_t> transforms;
    transform(children_.begin(), children_.end(), back_inserter(transforms),
              [depth, leaf_function, collect](const TreeNode& child) {
                  return child.recursive(depth - 1, leaf_function, collect);
              });

    // Collect tranformations.
    return collect(transforms);
}

// Grow by adding availalble gates to children.
void TreeNode::grow() {
    assert(children_.empty());
    const auto& avail_gates = scheduler_->get_avail_gates();
    for (size_t gate_idx : avail_gates) {
        children_.push_back(
            TreeNode{gate_idx, router().clone(), scheduler().clone()});
    }
}

void TreeNode::grow_if_needed() {
    if (is_leaf()) {
        grow();
    }
}

Dora::Dora(unique_ptr<Topology> topo, json& conf)
    : Greedy(move(topo), conf), depth(json_get<int>(conf, "depth")) {}

Dora::Dora(const Dora& other) : Greedy(other), depth(other.depth) {}

Dora::Dora(Dora&& other) : Greedy(other), depth(other.depth) {}

unique_ptr<SchedulerBase> Dora::clone() const {
    return make_unique<Dora>(*this);
}

void Dora::assign_gates(unique_ptr<QFTRouter> router) {
    auto total_gates = topo_->get_num_gates();

    Tqdm bar{total_gates};
    vector<TreeNode> next_trees;

    // For each step.
    for (size_t idx = 0; idx < total_gates; ++idx, bar.add()) {
        auto avail_gates = topo_->get_avail_gates();

        // Generate heuristic trees if not present.
        // Since router and this both outlive update_next_trees,
        // this usage is safe.
        update_next_trees(*router, *this, avail_gates, next_trees);

        // Calcuate each tree's costs and find the best one (smallest cost).
        vector<size_t> costs;
        transform(
            next_trees.begin(), next_trees.end(), back_inserter(costs),
            [this](const TreeNode& root) { return root.best_cost(depth); });

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
        next_trees = move(selected_tree.children());

        route_one_gate(*router, gate_idx);
    }
}

// Check if ids are matched with chidren's ids.
static void children_match_ids(const vector<size_t>& ids,
                               const vector<TreeNode>& children) {
    unordered_set<size_t> all_children{ids.begin(), ids.end()};

    assert(all_children.size() == children.size());
    for ([[maybe_unused]] const auto& child : children) {
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
                             vector<TreeNode>& next_trees) const {
    if (next_trees.empty()) [[unlikely]] {
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
void Dora::update_tree_recursive(int remaining_depth, TreeNode& root) const {
    const auto& avail_gates = root.scheduler().get_avail_gates();

    if (remaining_depth <= 0 || avail_gates.empty()) {
        return;
    }

    // If the heuristic tree has reached the leaf, extend it.
    root.grow_if_needed();

    // Update children heuristic search tree.
    root_match_avail_gates(root.scheduler(), root);
    for (auto& child_node : root.children()) {
        update_tree_recursive(remaining_depth - 1, child_node);
    }
}
