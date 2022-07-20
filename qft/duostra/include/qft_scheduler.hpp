#pragma once

#include <memory>
#include <utility>
#include <vector>
#include "q_device.hpp"
#include "qft_router.hpp"
#include "topo.hpp"

namespace scheduler {
using namespace std;
using namespace topo;

class Base {
   public:
    Base(const Base& other) = delete;
    Base(unique_ptr<Topology>&& topo) noexcept;
    Base(Base&& other) noexcept;
    virtual ~Base() {}

    virtual void assign_gates(unique_ptr<QFTRouter> router);

    void sort();

    void write_assembly(ostream& out) const;
    void to_json(json& j) const;

    unsigned get_final_cost() const;
    unsigned get_total_time() const;
    unsigned get_swap_num() const;

    const vector<device::Operation>& get_operations() const;
    size_t ops_cost() const;

   protected:
    unique_ptr<topo::Topology> topo_;
    vector<device::Operation> ops_;
    bool sorted_ = false;

    void route_gates(QFTRouter& router, size_t gate_idx);
    unsigned get_executable(QFTRouter& router,
                            vector<unsigned> wait_list) const;
};

class Random : public Base {
   public:
    Random(unique_ptr<topo::Topology>&& topo) : Base(move(topo)) {}
    Random(const Random& other) = delete;
    Random(Random&& other) = delete;
    ~Random() override {}

    void assign_gates(unique_ptr<QFTRouter> router) override;
};

class Static : public Base {
   public:
    Static(unique_ptr<topo::Topology>&& topo) : Base(move(topo)) {}
    Static(const Static& other) = delete;
    Static(Static&& other) = delete;
    ~Static() override {}

    void assign_gates(unique_ptr<QFTRouter> router) override;
};

class Conf {
   public:
    Conf()
        : avail_typ(true),
          cost_typ(false),
          candidates(UINT_MAX),
          apsp_coef(1) {}
    bool avail_typ;  // true is max, false is min
    bool cost_typ;   // true is max, false is min
    unsigned candidates, apsp_coef;
};

class Greedy : public Base {
   public:
    Greedy(unique_ptr<topo::Topology>&& topo, json& conf) noexcept;
    Greedy(const Greedy& other) = delete;
    Greedy(Greedy&& other) = delete;
    ~Greedy() override {}

    void assign_gates(unique_ptr<QFTRouter> router) override;
    unsigned greedy_fallback(const QFTRouter& router,
                             const std::vector<unsigned>& wait_list,
                             unsigned gate_idx) const;

   protected:
    Conf conf_;
};

class Onion : public Greedy {
   public:
    Onion(unique_ptr<Topology>&& topo, json& conf)
        : Greedy(move(topo), conf),
          first_mode_(json_get<bool>(conf, "layer_from_first")) {}
    Onion(const Onion& other) = delete;
    Onion(Onion&& other) = delete;
    ~Onion() override {}

    void assign_gates(unique_ptr<QFTRouter> router) override;

   private:
    bool first_mode_;
    Conf conf_;
};

struct PathsCosts {
    vector<size_t> path;
    size_t cost;
};

class Dora : public Base {
   public:
    Dora(unique_ptr<Topology>&& topo, json& conf)
        : Base(move(topo)), depth(json_get<int>(conf, "depth")) {}

    const size_t depth;
    void assign_gates(unique_ptr<QFTRouter> router) override;

   private:
    vector<PathsCosts> paths_costs(size_t depth,
                                   const vector<size_t>& path_so_far,
                                   unique_ptr<Topology> topo,
                                   unique_ptr<QFTRouter> router) const;
};

unique_ptr<Base> get(const string& typ,
                     unique_ptr<Topology>&& topo,
                     json& conf);
}  // namespace scheduler
