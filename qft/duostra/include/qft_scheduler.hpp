#pragma once

#include <memory>
#include <unordered_map>
#include <utility>
#include <vector>
#include "q_device.hpp"
#include "qft_router.hpp"
#include "topo.hpp"

namespace scheduler {
using namespace std;
using namespace topo;

class SchedulerBase {
   public:
    SchedulerBase(const SchedulerBase& other);
    SchedulerBase(unique_ptr<Topology> topo);
    SchedulerBase(SchedulerBase&& other);
    virtual ~SchedulerBase() {}

    topo::Topology& topo() { return *topo_; }
    const topo::Topology& topo() const { return *topo_; }

    virtual unique_ptr<SchedulerBase> clone() const;

    void assign_gates_and_sort(unique_ptr<QFTRouter> router) {
        assign_gates(std::move(router));
        sort();
    }

    void write_assembly(ostream& out) const;
    void to_json(json& j) const;

    size_t get_final_cost() const;
    size_t get_total_time() const;
    size_t get_swap_num() const;
    const vector<size_t>& get_avail_gates() const {
        return topo_->get_avail_gates();
    }

    const vector<device::Operation>& get_operations() const { return ops_; }
    size_t ops_cost() const;

    void route_one_gate(QFTRouter& router, size_t gate_idx);

   protected:
    unique_ptr<topo::Topology> topo_;
    vector<device::Operation> ops_;
    bool sorted_ = false;

    virtual void assign_gates(unique_ptr<QFTRouter> router);
    void sort();
    size_t get_executable(QFTRouter& router,
                          const vector<size_t>& wait_list) const;
};

class Random : public SchedulerBase {
   public:
    Random(unique_ptr<topo::Topology> topo);
    Random(const Random& other);
    Random(Random&& other);
    ~Random() override {}

    unique_ptr<SchedulerBase> clone() const override;

   protected:
    void assign_gates(unique_ptr<QFTRouter> router) override;
};

class Static : public SchedulerBase {
   public:
    Static(unique_ptr<topo::Topology> topo);
    Static(const Static& other);
    Static(Static&& other);
    ~Static() override {}

    unique_ptr<SchedulerBase> clone() const override;

   protected:
    void assign_gates(unique_ptr<QFTRouter> router) override;
};

struct GreedyConf {
    GreedyConf()
        : avail_typ(true),
          cost_typ(false),
          candidates(size_t(-1)),
          apsp_coef(1) {}

    GreedyConf(const json& conf);

    bool avail_typ;  // true is max, false is min
    bool cost_typ;   // true is max, false is min
    size_t candidates;
    size_t apsp_coef;
};

class Greedy : public SchedulerBase {
   public:
    Greedy(unique_ptr<topo::Topology> topo, const json& conf);
    Greedy(const Greedy& other);
    Greedy(Greedy&& other);
    ~Greedy() override {}

    unique_ptr<SchedulerBase> clone() const override;
    size_t greedy_fallback(const QFTRouter& router,
                           const std::vector<size_t>& wait_list,
                           size_t gate_idx) const;

   protected:
    GreedyConf conf_;

    void assign_gates(unique_ptr<QFTRouter> router) override;
};

class Onion : public Greedy {
   public:
    Onion(unique_ptr<Topology> topo, const json& conf);
    Onion(const Onion& other);
    Onion(Onion&& other);
    ~Onion() override {}

    unique_ptr<SchedulerBase> clone() const override;

   protected:
    bool first_mode_;

    void assign_gates(unique_ptr<QFTRouter> router) override;

    void assign_generation(
        QFTRouter& router,
        std::unordered_map<size_t, std::vector<size_t>>& gen_to_gates,
        Tqdm& bar,
        size_t& total_size);
    void assign_from_wait_list(QFTRouter& router,
                               vector<size_t>& wait_list,
                               size_t& total_size);
};

// This is a node of the heuristic search tree.
class TreeNode {
   public:
    TreeNode(size_t gate_idx,
             unique_ptr<QFTRouter> router,
             unique_ptr<SchedulerBase> scheduler);
    TreeNode(const TreeNode& other);
    TreeNode(TreeNode&& other);

    TreeNode& operator=(const TreeNode& other);
    TreeNode& operator=(TreeNode&& other);

    size_t tree_size(int depth) const;
    size_t best_cost(int depth) const;
    size_t gate_idx() const { return gate_idx_; }

    const QFTRouter& router() const { return *router_; }
    const SchedulerBase& scheduler() const { return *scheduler_; }

    vector<TreeNode>& children() { return children_; }
    const vector<TreeNode>& children() const { return children_; }

    bool is_leaf() const { return children_.empty(); }
    void grow_if_needed();

   private:
    size_t gate_idx_;
    vector<TreeNode> children_;
    unique_ptr<QFTRouter> router_;
    unique_ptr<SchedulerBase> scheduler_;

    void grow();
    void exec_route();

    template <typename T>
    T recursive(int depth,
                T function(const TreeNode&),
                T collect(const vector<T>&)) const;
};

class Dora : public Greedy {
   public:
    Dora(unique_ptr<Topology> topo, const json& conf);
    Dora(const Dora& other);
    Dora(Dora&& other);
    ~Dora() override {}

    const size_t look_ahead;

    unique_ptr<SchedulerBase> clone() const override;

   protected:
    void assign_gates(unique_ptr<QFTRouter> router) override;

    void update_next_trees(const QFTRouter& router,
                           const SchedulerBase& scheduler,
                           const vector<size_t>& next_ids,
                           vector<TreeNode>& next_trees) const;

    void update_tree_recursive(int remaining_depth, TreeNode& root) const;
};

unique_ptr<SchedulerBase> get(const string& typ,
                              unique_ptr<Topology> topo,
                              json& conf);
}  // namespace scheduler
