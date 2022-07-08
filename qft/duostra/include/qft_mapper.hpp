#pragma once

#include <algorithm>
#include <chrono>  // std::chrono::system_clock
#include <iostream>
#include <memory>
#include <random>  // std::default_random_engine
#include <string>
#include <tuple>
#include <vector>
#include "q_device.hpp"
#include "topo.hpp"
#include "tqdm.hpp"
#include "util.hpp"

class TopologyWrapperWithCandidate {
   public:
    TopologyWrapperWithCandidate(const topo::Topology& topo, unsigned candidate)
        : topo_(topo), candidate_(candidate) {}

    std::vector<unsigned> get_avail_gates() const {
        auto& gates = topo_.get_avail_gates();

        if (gates.size() < candidate_) {
            return gates;
        } else {
            return std::vector<unsigned>(gates.begin(),
                                         gates.begin() + candidate_);
        }

        return std::vector<unsigned>();
    }

   private:
    const topo::Topology& topo_;
    unsigned candidate_;
};

class QFTPlacer {
   public:
    QFTPlacer() {}
    QFTPlacer(const QFTPlacer& other) = delete;
    QFTPlacer(QFTPlacer&& other) = delete;

    std::vector<unsigned> place(device::Device& device, std::string& typ) {
        if (typ == "static") {
            return static_place(device);
        } else if (typ == "random") {
            return random_place(device);
        } else if (typ == "dfs") {
            return dfs_place(device);
        } else {
            std::cerr << typ << " is not a placer type." << std::endl;
            abort();
        }
    }

    std::vector<unsigned> random_place(device::Device& device) {
        std::vector<unsigned> assign;
        for (unsigned i = 0; i < device.get_num_qubits(); ++i) {
            assign.push_back(i);
        }
        unsigned seed =
            std::chrono::system_clock::now().time_since_epoch().count();

        shuffle(assign.begin(), assign.end(), std::default_random_engine(seed));
        return assign;
    }

    std::vector<unsigned> static_place(device::Device& device) {
        std::vector<unsigned> assign;
        for (unsigned i = 0; i < device.get_num_qubits(); ++i) {
            assign.push_back(i);
        }
        return assign;
    }

    std::vector<unsigned> dfs_place(device::Device& device) {
        std::vector<unsigned> assign;
        std::vector<bool> qubit_mark(device.get_num_qubits(), false);
        dfs_device(0, device, assign, qubit_mark);
        assert(assign.size() == device.get_num_qubits());
        return assign;
    }

   private:
    inline void dfs_device(unsigned current,
                           device::Device& device,
                           std::vector<unsigned>& assign,
                           std::vector<bool>& qubit_mark) {
        if (qubit_mark[current] == true) {
            std::cout << current << std::endl;
        }
        assert(qubit_mark[current] == false);
        qubit_mark[current] = true;
        assign.push_back(current);

        device::Qubit& q = device.get_qubit(current);
        std::vector<unsigned> adj_waitlist;

        for (unsigned i = 0; i < q.get_adj_list().size(); ++i) {
            unsigned adj = q.get_adj_list()[i];

            // already marked
            if (qubit_mark[adj]) {
                continue;
            }

            device::Qubit& adj_q = device.get_qubit(adj);
            assert(adj_q.get_adj_list().size() > 0);

            // corner
            if (adj_q.get_adj_list().size() == 1) {
                dfs_device(adj, device, assign, qubit_mark);
            } else {
                adj_waitlist.push_back(adj);
            }
        }
        for (unsigned i = 0; i < adj_waitlist.size(); ++i) {
            unsigned adj = adj_waitlist[i];
            if (qubit_mark[adj]) {
                continue;
            }
            dfs_device(adj, device, assign, qubit_mark);
        }
        return;
    }
};

class QFTRouter {
   public:
    QFTRouter(device::Device& device,
              std::string& typ,
              std::string& cost,
              bool orient)
        : _orient(orient), _device(device), _apsp(false) {
        if (typ == "apsp") {
            _apsp = true;
            _duostra = false;
        } else if (typ == "duostra") {
            _duostra = true;
        } else {
            std::cerr << typ << " is not a router type" << std::endl;
            abort();
        }

        if (cost == "end") {
            _greedy_type = true;
            _apsp = true;
        } else if (cost == "start") {
            _greedy_type = false;
        } else {
            std::cerr << cost << " is not a cost type" << std::endl;
            abort();
        }

        if (_apsp) {
            _device.init_apsp();
        }

        _topo2device.resize(device.get_num_qubits());
        for (unsigned i = 0; i < device.get_num_qubits(); ++i) {
            _topo2device[device.get_qubit(i).get_topo_qubit()] = i;
        }
    }

    QFTRouter(const QFTRouter& other)
        : _greedy_type(other._greedy_type),
          _duostra(other._duostra),
          _orient(other._orient),
          _device(other._device),
          _topo2device(other._topo2device),
          _apsp(other._apsp) {}

    QFTRouter(QFTRouter&& other)
        : _greedy_type(other._greedy_type),
          _duostra(other._duostra),
          _orient(other._orient),
          _device(other._device),
          _topo2device(std::move(other._topo2device)),
          _apsp(other._apsp) {}

    unsigned get_gate_cost(topo::Gate& gate,
                           bool min_max,
                           unsigned apsp_coef) const {
        std::tuple<unsigned, unsigned> device_qubits_idx =
            get_device_qubits_idx(gate);

        if (gate.get_type() == Operator::Single) {
            assert(std::get<1>(device_qubits_idx) == UINT_MAX);
            return _device.get_qubit(std::get<0>(device_qubits_idx))
                .get_avail_time();
        }

        unsigned q0_id = std::get<0>(device_qubits_idx);
        unsigned q1_id = std::get<1>(device_qubits_idx);
        device::Qubit& q0 = _device.get_qubit(q0_id);
        device::Qubit& q1 = _device.get_qubit(q1_id);
        unsigned apsp_cost = 0;
        if (_apsp) {
            apsp_cost = _device.get_shortest_cost(q0_id, q1_id);
            assert(apsp_cost == _device.get_shortest_cost(q1_id, q0_id));
        }
        unsigned avail =
            min_max ? std::max(q0.get_avail_time(), q1.get_avail_time())
                    : std::min(q0.get_avail_time(), q1.get_avail_time());
        return avail + apsp_cost / apsp_coef;
    }

    std::vector<device::Operation> assign_gate(topo::Gate& gate) {
        std::tuple<unsigned, unsigned> device_qubits_idx =
            get_device_qubits_idx(gate);

        if (gate.get_type() == Operator::Single) {
            assert(std::get<1>(device_qubits_idx) == UINT_MAX);
            device::Operation op = _device.execute_single(
                gate.get_type(), std::get<0>(device_qubits_idx));
            return std::vector<device::Operation>(1, op);
        }
        std::vector<device::Operation> op_list =
            _duostra ? _device.duostra_routing(gate.get_type(),
                                               device_qubits_idx, _orient)
                     : _device.apsp_routing(gate.get_type(), device_qubits_idx,
                                            _orient);
        std::vector<unsigned> change_list = _device.mapping();
#ifdef DEBUG
        std::vector<bool> checker(_topo2device.size(), false);
#endif
        for (unsigned i = 0; i < change_list.size();
             ++i)  // i is the idx of device qubit
        {
            unsigned topo_qubit_id = change_list[i];
            if (topo_qubit_id == UINT_MAX) {
                continue;
            }
#ifdef DEBUG
            assert(checker[i] == false);
            checker[i] = true;
#endif
            _topo2device[topo_qubit_id] = i;
        }
#ifdef DEBUG
        for (unsigned i = 0; i < checker.size(); ++i) {
            assert(checker[i]);
        }
        std::cout << "Gate: Q" << std::get<0>(gate.get_qubits()) << " Q"
                  << std::get<1>(gate.get_qubits()) << "\n";
        _device.print_device_state(std::cout);
#endif
        return op_list;
    }

    bool is_executable(topo::Gate& gate) const {
        if (gate.get_type() == Operator::Single) {
            assert(std::get<1>(gate.get_qubits()) == UINT_MAX);
            return true;
        }

        std::tuple<unsigned, unsigned> device_qubits_idx =
            get_device_qubits_idx(gate);
        assert(std::get<1>(device_qubits_idx) != UINT_MAX);
        device::Qubit& q0 = _device.get_qubit(std::get<0>(device_qubits_idx));
        device::Qubit& q1 = _device.get_qubit(std::get<1>(device_qubits_idx));
        return q0.is_adj(q1);
    }

   private:
    std::tuple<unsigned, unsigned> get_device_qubits_idx(
        topo::Gate& gate) const {
        unsigned topo_idx_q0 =
            std::get<0>(gate.get_qubits());  // get operation qubit index of
                                             // gate in topology
        unsigned device_idx_q0 =
            _topo2device[topo_idx_q0];  // get device qubit index of the gate

        unsigned device_idx_q1 = UINT_MAX;

        if (gate.get_type() != Operator::Single) {
            unsigned topo_idx_q1 =
                std::get<1>(gate.get_qubits());  // get operation qubit index of
                                                 // gate in topology
            assert(topo_idx_q1 != UINT_MAX);
            device_idx_q1 = _topo2device[topo_idx_q1];  // get device qubit
                                                        // index of the gate
        }
        return std::make_tuple(device_idx_q0, device_idx_q1);
    }

    bool _greedy_type;
    bool _duostra, _orient;
    device::Device& _device;
    std::vector<unsigned> _topo2device;
    bool _apsp;
};

namespace scheduler {
class Base {
   public:
    Base(std::unique_ptr<topo::Topology>&& topo) : topo_(std::move(topo)) {}
    Base(const Base& other) = delete;
    Base(Base&& other) = delete;
    virtual ~Base() {}

    virtual void assign_gates(std::unique_ptr<QFTRouter> router);

    void write_assembly(std::ostream& out) {
        std::sort(ops_.begin(), ops_.end(), device::op_order);

        for (unsigned i = 0; i < ops_.size(); ++i) {
            device::Operation& op = ops_[i];
            switch (op.get_operator()) {
                case Operator::CX:
                    out << "CX ";
                    break;
                case Operator::Swap:
                    out << "Swap ";
                    break;
                case Operator::Single:
                    out << "Single ";
                    break;
                default:
                    assert(false);
            }
            std::tuple<unsigned, unsigned> qubits = op.get_qubits();
            out << "Q[" << std::get<0>(qubits) << "] Q[" << std::get<1>(qubits)
                << "]; ";
            out << "(" << op.get_op_time() << "," << op.get_cost() << ")\n";
        }
    }

    void to_json(json& j) {
        std::sort(ops_.begin(), ops_.end(), device::op_order);

        json o;
        for (unsigned i = 0; i < ops_.size(); ++i) {
            device::Operation& op = ops_[i];
            json buf = op;
            o.push_back(buf);
        }
        j["Operations"] = o;
    }

    unsigned get_final_cost() {
        std::sort(ops_.begin(), ops_.end(), device::op_order);

        return ops_[ops_.size() - 1].get_cost();
    }

    unsigned get_total_time() {
        std::sort(ops_.begin(), ops_.end(), device::op_order);

        unsigned ret = 0;
        for (unsigned i = 0; i < ops_.size(); ++i) {
            std::tuple<unsigned, unsigned> dur = ops_[i].get_duration();
            ret += std::get<1>(dur) - std::get<0>(dur);
        }
        return ret;
    }

    unsigned get_swap_num() {
        std::sort(ops_.begin(), ops_.end(), device::op_order);
        unsigned ret = 0;
        for (unsigned i = 0; i < ops_.size(); ++i) {
            if (ops_[i].get_operator() == Operator::Swap) {
                ret += 1;
            }
        }
        return ret;
    }

    const std::vector<device::Operation>& get_operations() { return ops_; }

   protected:
    std::unique_ptr<topo::Topology> topo_;
    std::vector<device::Operation> ops_;

    unsigned get_executable(QFTRouter& router, std::vector<unsigned> waitlist) {
        for (unsigned gate_idx : waitlist) {
            if (router.is_executable(topo_->get_gate(gate_idx))) {
                return gate_idx;
            }
        }
        return UINT_MAX;
    }
};

class Random : public Base {
   public:
    Random(std::unique_ptr<topo::Topology>&& topo) : Base(std::move(topo)) {}
    Random(const Random& other) = delete;
    Random(Random&& other) = delete;
    ~Random() override {}

    void assign_gates(std::unique_ptr<QFTRouter> router) override;
};

class Static : public Base {
   public:
    Static(std::unique_ptr<topo::Topology>&& topo) : Base(std::move(topo)) {}
    Static(const Static& other) = delete;
    Static(Static&& other) = delete;
    ~Static() override {}

    void assign_gates(std::unique_ptr<QFTRouter> router) override;
};

class Onion : public Base {
   public:
    Onion(std::unique_ptr<topo::Topology>&& topo, json& conf)
        : Base(std::move(topo)),
          first_mode_(json_get<bool>(conf, "layer_from_first")),
          cost_typ_(json_get<bool>(conf, "cost")) {}
    Onion(const Onion& other) = delete;
    Onion(Onion&& other) = delete;
    ~Onion() override {}

    void assign_gates(std::unique_ptr<QFTRouter> router) override;

   private:
    bool first_mode_;
    bool cost_typ_;  // true is max, false is min
};

class Greedy : public Base {
   public:
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
    Greedy(std::unique_ptr<topo::Topology>&& topo, json& conf)
        : Base(std::move(topo)) {
        int candidates = json_get<int>(conf, "candidates");
        if (candidates > 0) {
            _conf.candidates = candidates;
        }
        _conf.apsp_coef = json_get<unsigned>(conf, "apsp_coef");
        std::string avail_typ = json_get<std::string>(conf, "avail");
        if (avail_typ == "min") {
            _conf.avail_typ = false;
        } else if (avail_typ == "max") {
            _conf.avail_typ = true;
        } else {
            std::cerr << "\"min_max\" can only be \"min\" or \"max\"."
                      << std::endl;
            abort();
        }
        std::string cost_typ = json_get<std::string>(conf, "cost");
        if (cost_typ == "min") {
            _conf.cost_typ = false;
        } else if (cost_typ == "max") {
            _conf.cost_typ = true;
        } else {
            std::cerr << "\"min_max\" can only be \"min\" or \"max\"."
                      << std::endl;
            abort();
        }
    }
    Greedy(const Greedy& other) = delete;
    Greedy(Greedy&& other) = delete;
    ~Greedy() override {}

    void assign_gates(std::unique_ptr<QFTRouter> router) override;

   private:
    Conf _conf;
};

class BlockadeRevitalizer : public Base {
   public:
    BlockadeRevitalizer(std::unique_ptr<topo::Topology>&& topo, json& conf)
        : Base(std::move(topo)), depth(json_get<int>(conf, "depth")) {}

    const size_t depth;
    void assign_gates(std::unique_ptr<QFTRouter> router) override;

   private:
    void assign_one_gate(QFTRouter router);
};

std::unique_ptr<Base> get(const std::string& typ,
                          std::unique_ptr<topo::Topology>&& topo,
                          json& conf);
}  // namespace scheduler
