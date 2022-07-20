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
    SchedulerBase(const SchedulerBase& other) noexcept;
    SchedulerBase(unique_ptr<Topology> topo) noexcept;
    SchedulerBase(SchedulerBase&& other) noexcept;
    virtual ~SchedulerBase() {}

    topo::Topology& topo() { return *topo_; }
    const topo::Topology& topo() const { return *topo_; }

    virtual unique_ptr<SchedulerBase> clone() const;
    virtual void assign_gates(unique_ptr<QFTRouter> router);

    void sort();

    void write_assembly(ostream& out) const;
    void to_json(json& j) const;

    size_t get_final_cost() const;
    size_t get_total_time() const;
    size_t get_swap_num() const;
    const vector<size_t>& get_avail_gates() const;

    const vector<device::Operation>& get_operations() const;
    size_t ops_cost() const;

    void route_one_gate(QFTRouter& router, size_t gate_idx);

   protected:
    unique_ptr<topo::Topology> topo_;
    vector<device::Operation> ops_;
    bool sorted_ = false;

    size_t get_executable(QFTRouter& router, vector<size_t> wait_list) const;
};

class Random : public SchedulerBase {
   public:
    Random(unique_ptr<topo::Topology> topo) noexcept;
    Random(const Random& other) noexcept;
    Random(Random&& other) noexcept;
    ~Random() override {}

    unique_ptr<SchedulerBase> clone() const override;

    void assign_gates(unique_ptr<QFTRouter> router) override;
};

class Static : public SchedulerBase {
   public:
    Static(unique_ptr<topo::Topology> topo) noexcept;
    Static(const Static& other) noexcept;
    Static(Static&& other) noexcept;
    ~Static() override {}

    unique_ptr<SchedulerBase> clone() const override;
    void assign_gates(unique_ptr<QFTRouter> router) override;
};

struct Conf {
    Conf() noexcept
        : avail_typ(true),
          cost_typ(false),
          candidates(size_t(-1)),
          apsp_coef(1) {}
    bool avail_typ;  // true is max, false is min
    bool cost_typ;   // true is max, false is min
    size_t candidates;
    size_t apsp_coef;
};

class Greedy : public SchedulerBase {
   public:
    Greedy(unique_ptr<topo::Topology> topo, json& conf) noexcept;
    Greedy(const Greedy& other) noexcept;
    Greedy(Greedy&& other) noexcept;
    ~Greedy() override {}

    unique_ptr<SchedulerBase> clone() const override;
    void assign_gates(unique_ptr<QFTRouter> router) override;
    size_t greedy_fallback(const QFTRouter& router,
                           const std::vector<size_t>& wait_list,
                           size_t gate_idx) const;

   protected:
    Conf conf_;
};

class Onion : public Greedy {
   public:
    Onion(unique_ptr<Topology> topo, json& conf) noexcept;
    Onion(const Onion& other) noexcept;
    Onion(Onion&& other) noexcept;
    ~Onion() override {}

    unique_ptr<SchedulerBase> clone() const override;
    void assign_gates(unique_ptr<QFTRouter> router) override;

   private:
    bool first_mode_;
    Conf conf_;
};

// This is a node of the heuristic search tree.
class TreeNode {
   public:
    TreeNode(size_t gate_idx,
             unique_ptr<QFTRouter> router,
             unique_ptr<SchedulerBase> scheduler) noexcept;
    TreeNode(const TreeNode& other) noexcept;
    TreeNode& operator=(const TreeNode& other) noexcept;

    size_t cost(int depth) const;
    size_t gate_idx() const { return gate_idx_; }

    QFTRouter& router() { return *router_; }
    SchedulerBase& scheduler() { return *scheduler_; }

    vector<TreeNode>& children() { return children_; }
    const vector<TreeNode>& children() const { return children_; }

    bool is_leaf() const { return children_.empty(); }
    void grow();

   private:
    size_t gate_idx_;
    vector<TreeNode> children_;
    unique_ptr<QFTRouter> router_;
    unique_ptr<SchedulerBase> scheduler_;

    void exec_route();
};

class Dora : public Greedy {
   public:
    Dora(unique_ptr<Topology> topo, json& conf) noexcept;

    Dora(const Dora& other) noexcept;
    Dora(Dora&& other) noexcept;

    const size_t depth;

    unique_ptr<SchedulerBase> clone() const override;
    void assign_gates(unique_ptr<QFTRouter> router) override;

   protected:
    void update_next_trees(const QFTRouter& router,
                           const SchedulerBase& scheduler,
                           const vector<size_t>& next_ids,
                           vector<TreeNode>& next_trees);
    void update_tree_recursive(int remaining_depth, TreeNode& root);
};

unique_ptr<SchedulerBase> get(const string& typ,
                              unique_ptr<Topology> topo,
                              json& conf);
}  // namespace scheduler
