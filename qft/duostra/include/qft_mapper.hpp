#pragma once

#include "q_device.hpp"
#include "topo.hpp"
#include "tqdm.hpp"
#include <algorithm>
#include <chrono> // std::chrono::system_clock
#include <iostream>
#include <random> // std::default_random_engine
#include <span>
#include <string>
#include <tuple>
#include <vector>

class TopologyWrapperWithCandidate {
  public:
    TopologyWrapperWithCandidate(topo::Topology &topo, unsigned candidate)
        : topo_(topo), candidate_(candidate) {}

    std::vector<unsigned> get_avail_gates() const {
        auto &gates = topo_.get_avail_gates();

        if (gates.size() < candidate_) {
            return gates;
        } else {
            return std::vector<unsigned>(gates.begin(),
                                         gates.begin() + candidate_);
        }

        return std::vector<unsigned>();
    }

  private:
    topo::Topology &topo_;
    unsigned candidate_;
};

class QFTPlacer {
  public:
    QFTPlacer() {}
    QFTPlacer(const QFTPlacer &other) = delete;
    QFTPlacer(QFTPlacer &&other) = delete;

    std::vector<unsigned> place(device::Device &device, std::string &typ) {
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

    std::vector<unsigned> random_place(device::Device &device) {
        std::vector<unsigned> assign;
        for (unsigned i = 0; i < device.get_num_qubits(); ++i) {
            assign.push_back(i);
        }
        unsigned seed =
            std::chrono::system_clock::now().time_since_epoch().count();

        shuffle(assign.begin(), assign.end(), std::default_random_engine(seed));
        return assign;
    }

    std::vector<unsigned> static_place(device::Device &device) {
        std::vector<unsigned> assign;
        for (unsigned i = 0; i < device.get_num_qubits(); ++i) {
            assign.push_back(i);
        }
        return assign;
    }

    std::vector<unsigned> dfs_place(device::Device &device) {
        std::vector<unsigned> assign;
        std::vector<bool> qubit_mark(device.get_num_qubits(), false);
        dfs_device(0, device, assign, qubit_mark);
        assert(assign.size() == device.get_num_qubits());
        return assign;
    }

  private:
    inline void dfs_device(unsigned current, device::Device &device,
                           std::vector<unsigned> &assign,
                           std::vector<bool> &qubit_mark) {
        if (qubit_mark[current] == true) {
            std::cout << current << std::endl;
        }
        assert(qubit_mark[current] == false);
        qubit_mark[current] = true;
        assign.push_back(current);

        device::Qubit &q = device.get_qubit(current);
        std::vector<unsigned> adj_waitlist;

        for (unsigned i = 0; i < q.get_adj_list().size(); ++i) {
            unsigned adj = q.get_adj_list()[i];

            // already marked
            if (qubit_mark[adj]) {
                continue;
            }

            device::Qubit &adj_q = device.get_qubit(adj);
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
    QFTRouter(device::Device &device, std::string &typ, std::string &cost)
        : _device(device), _apsp(false) {
        if (typ == "naive") {
            _orient = false;
        } else if (typ == "orientation") {
            _orient = true;
        } else {
            std::cerr << typ << " is not a router type" << std::endl;
            abort();
        }

        if (cost == "end") {
            _greedy_type = true;
            _shortest_path = _device.init_apsp();
            _apsp = true;
        } else if (cost == "start") {
            _greedy_type = false;
        } else {
            std::cerr << cost << " is not a cost type" << std::endl;
            abort();
        }

        _topo2device.resize(device.get_num_qubits());
        for (unsigned i = 0; i < device.get_num_qubits(); ++i) {
            _topo2device[device.get_qubit(i).get_topo_qubit()] = i;
        }
    }
    QFTRouter(const QFTRouter &other) = delete;
    QFTRouter(QFTRouter &&other)
        : _greedy_type(other._greedy_type), _orient(other._orient),
          _device(other._device), _topo2device(std::move(other._topo2device)),
          _apsp(other._apsp), _shortest_path(std::move(other._shortest_path)) {}

    unsigned get_gate_cost(topo::Gate &gate) const {
        std::tuple<unsigned, unsigned> device_qubits_idx =
            get_device_qubits_idx(gate);

        if (gate.get_type() == Operator::Single) {
            assert(std::get<1>(device_qubits_idx) == UINT_MAX);
            return _device.get_qubit(std::get<0>(device_qubits_idx))
                .get_avail_time();
        }

        unsigned q0_id = std::get<0>(device_qubits_idx);
        unsigned q1_id = std::get<1>(device_qubits_idx);
        device::Qubit &q0 = _device.get_qubit(q0_id);
        device::Qubit &q1 = _device.get_qubit(q1_id);
        unsigned apsp_cost = 0;
        if (_apsp) {
            apsp_cost = _shortest_path[q0_id][q1_id];
            assert(apsp_cost == _shortest_path[q1_id][q0_id]);
        }
        return std::max(q0.get_avail_time(), q1.get_avail_time()) + apsp_cost;
    }

    std::vector<device::Operation> assign_gate(topo::Gate &gate) {
        std::tuple<unsigned, unsigned> device_qubits_idx =
            get_device_qubits_idx(gate);

        if (gate.get_type() == Operator::Single) {
            assert(std::get<1>(device_qubits_idx) == UINT_MAX);
            device::Operation op = _device.execute_single(
                gate.get_type(), std::get<0>(device_qubits_idx));
            return std::vector<device::Operation>(1, op);
        }
        std::vector<device::Operation> op_list =
            _device.routing(gate.get_type(), device_qubits_idx, _orient);
        std::vector<unsigned> change_list = _device.mapping();
#ifdef DEBUG
        std::vector<bool> checker(_topo2device.size(), false);
#endif
        for (unsigned i = 0; i < change_list.size();
             ++i) // i is the idx of device qubit
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

    bool is_executable(topo::Gate &gate) const {
        if (gate.get_type() == Operator::Single) {
            assert(std::get<1>(gate.get_qubits()) == UINT_MAX);
            return true;
        }

        std::tuple<unsigned, unsigned> device_qubits_idx =
            get_device_qubits_idx(gate);
        assert(std::get<1>(device_qubits_idx) != UINT_MAX);
        device::Qubit &q0 = _device.get_qubit(std::get<0>(device_qubits_idx));
        device::Qubit &q1 = _device.get_qubit(std::get<1>(device_qubits_idx));
        return q0.is_adj(q1);
    }

  private:
    std::tuple<unsigned, unsigned>
    get_device_qubits_idx(topo::Gate &gate) const {
        unsigned topo_idx_q0 = std::get<0>(
            gate.get_qubits()); // get operation qubit index of gate in topology
        unsigned device_idx_q0 =
            _topo2device[topo_idx_q0]; // get device qubit index of the gate

        unsigned device_idx_q1 = UINT_MAX;

        if (gate.get_type() != Operator::Single) {
            unsigned topo_idx_q1 =
                std::get<1>(gate.get_qubits()); // get operation qubit index of
                                                // gate in topology
            assert(topo_idx_q1 != UINT_MAX);
            device_idx_q1 =
                _topo2device[topo_idx_q1]; // get device qubit index of the gate
        }
        return std::make_tuple(device_idx_q0, device_idx_q1);
    }

    bool _greedy_type;
    bool _orient;
    device::Device &_device;
    std::vector<unsigned> _topo2device;
    bool _apsp;
    std::vector<std::vector<unsigned>> _shortest_path;
};

class QFTScheduler {
  public:
    QFTScheduler(topo::Topology &topo, unsigned candidates)
        : _topo(topo), _cand(candidates) {}
    QFTScheduler(const QFTScheduler &other) = delete;
    QFTScheduler(QFTScheduler &&other)
        : _topo(other._topo), _cand(other._cand), _ops(std::move(other._ops)) {}

    void assign_gates(QFTRouter &router, std::string &typ) {
        if (typ == "random") {
            return assign_gates_random(router);
        } else if (typ == "dp") {
            return assign_gates_dp(router);
        } else if (typ == "static") {
            return assign_gates_static(router);
        } else if (typ == "greedy") {
            return assign_gates_greedy(router, _cand);
        } else if (typ == "old") {
            return assign_gates_old(router);
        } else {
            std::cerr << typ << " is not a scheduler type" << std::endl;
            abort();
        }
    }

    void assign_gates_random(QFTRouter &router) {
#ifdef DEBUG
        unsigned count = 0;
#endif

        for (Tqdm bar{_topo.get_num_gates()}; !_topo.get_avail_gates().empty();
             bar.add()) {
            auto &wait_list = _topo.get_avail_gates();
            assert(wait_list.size() > 0);
#ifndef DEBUG
            srand(std::chrono::system_clock::now().time_since_epoch().count());
#endif
            unsigned choose = rand() % wait_list.size();
            topo::Gate &gate = _topo.get_gate(wait_list[choose]);
            std::vector<device::Operation> ops = router.assign_gate(gate);
            _ops.insert(_ops.end(), ops.begin(),
                        ops.end()); // append operations to the back
#ifdef DEBUG
            std::cout << wait_list << " " << wait_list[choose] << "\n\n";
            count++;
#endif
            _topo.update_avail_gates(wait_list[choose]);
        }
#ifdef DEBUG
        assert(count == _topo.get_num_gates());
#endif
    }

    void assign_gates_static(QFTRouter &router) {
#ifdef DEBUG
        unsigned count = 0;
#endif

        for (Tqdm bar{_topo.get_num_gates()}; !_topo.get_avail_gates().empty();
             bar.add()) {
            auto &wait_list = _topo.get_avail_gates();
            assert(wait_list.size() > 0);
            unsigned gate_idx = get_executable(router, wait_list);
            if (gate_idx == UINT_MAX) {
                gate_idx = wait_list[0];
            }
            topo::Gate &gate = _topo.get_gate(gate_idx);
            std::vector<device::Operation> ops = router.assign_gate(gate);
            _ops.insert(_ops.end(), ops.begin(),
                        ops.end()); // append operations to the back
#ifdef DEBUG
            count++;
#endif
            _topo.update_avail_gates(gate_idx);
        }
#ifdef DEBUG
        assert(count == _topo.get_num_gates());
#endif
    }

    void assign_gates_dp(QFTRouter &router) {
        for (Tqdm bar{_topo.get_num_gates()}; !_topo.get_avail_gates().empty();
             bar.add()) {
        }
    }

    void assign_gates_old(QFTRouter &router) {
        Tqdm bar{_topo.get_num_gates()};
        for (unsigned i = 0; i < _topo.get_num_gates(); ++i) {
            bar.add();
            topo::Gate &gate = _topo.get_gate(i);
            std::vector<device::Operation> ops = router.assign_gate(gate);
            _ops.insert(_ops.end(), ops.begin(),
                        ops.end()); // append operations to the back
            _topo.update_avail_gates(i);
        }
    }

    void assign_gates_greedy(QFTRouter &router, int candidates) {
#ifdef DEBUG
        unsigned count = 0;
#endif
        auto topo_wrap = TopologyWrapperWithCandidate(_topo, candidates);

        for (Tqdm bar{_topo.get_num_gates()};
             !topo_wrap.get_avail_gates().empty(); bar.add()) {
            auto wait_list = topo_wrap.get_avail_gates();
            assert(wait_list.size() > 0);

            unsigned gate_idx = get_executable(router, wait_list);
            if (gate_idx == UINT_MAX) {
                std::vector<unsigned> cost_list(wait_list.size(), 0);
                for (unsigned i = 0; i < wait_list.size(); ++i) {
                    topo::Gate &gate = _topo.get_gate(wait_list[i]);
                    unsigned cost = router.get_gate_cost(gate);
                    cost_list[i] = cost;
                }
                gate_idx = wait_list[std::min_element(cost_list.begin(),
                                                      cost_list.end()) -
                                     cost_list.begin()];
            }
            topo::Gate &gate = _topo.get_gate(gate_idx);
            std::vector<device::Operation> ops = router.assign_gate(gate);
            _ops.insert(_ops.end(), ops.begin(),
                        ops.end()); // append operations to the back
#ifdef DEBUG
            std::cout << "waitlist: " << wait_list << " " << gate_idx << "\n\n";
            count++;
#endif
            _topo.update_avail_gates(gate_idx);
        }
#ifdef DEBUG
        assert(count == _topo.get_num_gates());
#endif
    }

    void write_assembly(std::ostream &out) {
        std::sort(_ops.begin(), _ops.end(), device::op_order);

        for (unsigned i = 0; i < _ops.size(); ++i) {
            device::Operation &op = _ops[i];
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

    void to_json(json &j) {
        std::sort(_ops.begin(), _ops.end(), device::op_order);

        json o;
        for (unsigned i = 0; i < _ops.size(); ++i) {
            device::Operation &op = _ops[i];
            json buf = op;
            o.push_back(buf);
        }
        j["Operations"] = o;
    }

    unsigned get_final_cost() {
        std::sort(_ops.begin(), _ops.end(), device::op_order);

        return _ops[_ops.size() - 1].get_cost();
    }

    unsigned get_total_time() {
        std::sort(_ops.begin(), _ops.end(), device::op_order);

        unsigned ret = 0;
        for (unsigned i = 0; i < _ops.size(); ++i) {
            std::tuple<unsigned, unsigned> dur = _ops[i].get_duration();
            ret += std::get<1>(dur) - std::get<0>(dur);
        }
        return ret;
    }

    unsigned get_swap_num() {
        std::sort(_ops.begin(), _ops.end(), device::op_order);
        unsigned ret = 0;
        for (unsigned i = 0; i < _ops.size(); ++i) {
            if (_ops[i].get_operator() == Operator::Swap) {
                ret += 1;
            }
        }
        return ret;
    }

    std::vector<device::Operation> &get_operations() { return _ops; }

  private:
    topo::Topology &_topo;
    unsigned _cand;
    std::vector<device::Operation> _ops;

    unsigned get_executable(QFTRouter &router, std::vector<unsigned> waitlist) {
        for (unsigned gate_idx : waitlist) {
            if (router.is_executable(_topo.get_gate(gate_idx))) {
                return gate_idx;
            }
        }
        return UINT_MAX;
    }
};
