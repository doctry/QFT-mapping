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
    topo::Topology& topo_;
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
        : _device(device) {
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
            _device.init_apsp();
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
          _device(other._device), _topo2device(std::move(other._topo2device)) {}

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
        unsigned apsp_cost = _device.get_apsp_cost(q0_id, q1_id);
        assert(apsp_cost == _device.get_apsp_cost(q1_id, q0_id));
        return std::max(q0.get_avail_time(), q1.get_avail_time()) +
               _greedy_type * apsp_cost;
    }

    void assign_gate(topo::Gate &gate) {
        std::tuple<unsigned, unsigned> device_qubits_idx =
            get_device_qubits_idx(gate);

        if (gate.get_type() == Operator::Single) {
            assert(std::get<1>(device_qubits_idx) == UINT_MAX);
            _device.execute_single(gate.get_type(),
                                   std::get<0>(device_qubits_idx));
        } else {
            std::vector<unsigned> change_list =
                _device.routing(gate.get_type(), device_qubits_idx, _orient);
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
        }
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
};

class QFTScheduler {
  public:
    QFTScheduler(topo::Topology &topo, unsigned candidates)
        : _topo(topo), _cand(candidates) {}
    QFTScheduler(const QFTScheduler &other) = delete;
    QFTScheduler(QFTScheduler &&other)
        : _topo(other._topo), _cand(other._cand) {}

    void assign_gates(QFTRouter &router, std::string &typ) {
        if (typ == "random") {
            return assign_gates_random(router);
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
        Tqdm bar(_topo.get_num_gates());
        while (!_topo.get_avail_gates().empty()) {
            bar.add();
            auto &wait_list = _topo.get_avail_gates();
            assert(wait_list.size() > 0);
#ifndef DEBUG
            srand(std::chrono::system_clock::now().time_since_epoch().count());
#endif
            unsigned choose = rand() % wait_list.size();
            topo::Gate &gate = _topo.get_gate(wait_list[choose]);
            router.assign_gate(gate);
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
        Tqdm bar(_topo.get_num_gates());
        while (!_topo.get_avail_gates().empty()) {
            bar.add();
            auto &wait_list = _topo.get_avail_gates();
            assert(wait_list.size() > 0);
            unsigned gate_idx = get_executable(router, wait_list);
            if (gate_idx == UINT_MAX) {
                gate_idx = wait_list[0];
            }
            topo::Gate &gate = _topo.get_gate(gate_idx);
            router.assign_gate(gate);
#ifdef DEBUG
            count++;
#endif
            _topo.update_avail_gates(gate_idx);
        }
#ifdef DEBUG
        assert(count == _topo.get_num_gates());
#endif
    }

    void assign_gates_old(QFTRouter &router) {
        Tqdm bar(_topo.get_num_gates());
        for (unsigned i = 0; i < _topo.get_num_gates(); ++i) {
            bar.add();
            topo::Gate &gate = _topo.get_gate(i);
            router.assign_gate(gate);
            _topo.update_avail_gates(i);
        }
    }

    void assign_gates_greedy(QFTRouter &router, int candidates) {
#ifdef DEBUG
        unsigned count = 0;
#endif
        auto topo_wrap = TopologyWrapperWithCandidate(_topo, candidates);

        Tqdm bar(_topo.get_num_gates());
        while (!topo_wrap.get_avail_gates().empty()) {
            bar.add();
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
            router.assign_gate(gate);
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

  private:
    topo::Topology &_topo;
    unsigned _cand;

    unsigned get_executable(QFTRouter &router, std::vector<unsigned> waitlist) {
        for (unsigned gate_idx : waitlist) {
            if (router.is_executable(_topo.get_gate(gate_idx))) {
                return gate_idx;
            }
        }
        return UINT_MAX;
    }
};
