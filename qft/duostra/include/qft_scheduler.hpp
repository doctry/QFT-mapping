#pragma once

#include <functional>
#include <memory>
#include <unordered_map>
#include <utility>
#include <vector>
#include "q_device.hpp"
#include "qft_router.hpp"
#include "topo.hpp"
#include "util.hpp"

namespace scheduler {
using namespace std;
using namespace topo;

class Base {
   public:
    Base(const Base& other);
    Base(unique_ptr<Topology> topo);
    Base(Base&& other);
    virtual ~Base() {}

    Topology& topo() { return *topo_; }
    const topo::Topology& topo() const { return *topo_; }

    virtual unique_ptr<Base> clone() const;

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

    size_t get_executable(QFTRouter& router) const;

   protected:
    unique_ptr<topo::Topology> topo_;
    vector<device::Operation> ops_;
    bool sorted_ = false;

    virtual void assign_gates(unique_ptr<QFTRouter> router);
    void sort();
};

class Random : public Base {
   public:
    Random(unique_ptr<topo::Topology> topo);
    Random(const Random& other);
    Random(Random&& other);
    ~Random() override {}

    unique_ptr<Base> clone() const override;

   protected:
    void assign_gates(unique_ptr<QFTRouter> router) override;
};

class Static : public Base {
   public:
    Static(unique_ptr<topo::Topology> topo);
    Static(const Static& other);
    Static(Static&& other);
    ~Static() override {}

    unique_ptr<Base> clone() const override;

   protected:
    void assign_gates(unique_ptr<QFTRouter> router) override;
};

struct GreedyConf {
    GreedyConf()
        : avail_typ(true),
          cost_typ(false),
          candidates(ERROR_CODE),
          apsp_coef(1) {}

    GreedyConf(const json& conf);

    bool avail_typ;  // true is max, false is min
    bool cost_typ;   // true is max, false is min
    size_t candidates;
    size_t apsp_coef;
};

class Greedy : public Base {
   public:
    Greedy(unique_ptr<topo::Topology> topo, const json& conf);
    Greedy(const Greedy& other);
    Greedy(Greedy&& other);
    ~Greedy() override {}

    unique_ptr<Base> clone() const override;
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

    unique_ptr<Base> clone() const override;

   protected:
    bool first_mode_;

    void assign_gates(unique_ptr<QFTRouter> router) override;

    void assign_generation(
        QFTRouter& router,
        std::unordered_map<size_t, std::vector<size_t>>& gen_to_gates,
        TqdmWrapper& bar,
        size_t& total_size);
    void assign_from_wait_list(QFTRouter& router,
                               vector<size_t>& wait_list,
                               size_t& total_size);
};

struct TreeNodeConf {
    // Never cache any children unless children() is called.
    bool never_cache;
    // Execute the single gates when they are available.
    bool exec_single;
    // The number of childrens to consider,
    // selected with some ops_cost heuristic.
    size_t candidates;
};

// This is a node of the heuristic search tree.
class TreeNode {
   public:
    TreeNode(TreeNodeConf conf,
             size_t gate_idx,
             unique_ptr<QFTRouter> router,
             unique_ptr<Base> scheduler);
    TreeNode(TreeNodeConf,
             vector<size_t>&& gate_indices,
             unique_ptr<QFTRouter> router,
             unique_ptr<Base> scheduler);
    TreeNode(const TreeNode& other);
    TreeNode(TreeNode&& other);

    TreeNode& operator=(const TreeNode& other);
    TreeNode& operator=(TreeNode&& other);

    TreeNode best_child(int depth);

    size_t best_cost(int depth);
    size_t best_cost() const;

    const QFTRouter& router() const { return *router_; }
    const Base& scheduler() const { return *scheduler_; }

    const vector<size_t>& executed_gates() const { return gate_indices_; }

    bool is_leaf() const { return children_.empty(); }
    void grow_if_needed();

   private:
    TreeNodeConf conf_;

    // The head of the node.
    vector<size_t> gate_indices_;

    // Using vector to pointer so that frequent cache misses
    // won't be as bad in parallel code.
    vector<TreeNode> children_;

    // The state of duostra.
    unique_ptr<QFTRouter> router_;
    unique_ptr<Base> scheduler_;

    vector<TreeNode>& children();

    void grow();
    void route_internal_gates();
    size_t immediate_next() const;
};

class Dora : public Greedy {
   public:
    Dora(unique_ptr<Topology> topo, const json& conf);
    Dora(const Dora& other);
    Dora(Dora&& other);
    ~Dora() override {}

    const size_t look_ahead;

    unique_ptr<Base> clone() const override;

   protected:
    bool never_cache_;
    bool exec_single_;

    void assign_gates(unique_ptr<QFTRouter> router) override;
    void cache_only_when_necessary();
};

unique_ptr<Base> get(const string& typ, unique_ptr<Topology> topo, json& conf);
}  // namespace scheduler
