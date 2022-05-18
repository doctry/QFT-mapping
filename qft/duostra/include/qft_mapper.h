#pragma once

#include "topo.h"
#include "q_device.h"
#include <tuple>
#include <iostream>
#include <string>
#include <algorithm>
#include <vector>
#include <random> // std::default_random_engine
#include <chrono> // std::chrono::system_clock

class QFTPlacer
{
public:
    QFTPlacer() {}
    QFTPlacer(const QFTPlacer &other) = delete;
    QFTPlacer(QFTPlacer &&other) {}

    std::vector<unsigned> place(device::Device &device, std::string &typ)
    {
        if (typ == "static")
        {
            return static_place(device);
        }
        else if (typ == "random")
        {
            return random_place(device);
        }
        else if (typ == "dfs")
        {
            return dfs_place(device);
        }
        else
        {
            std::cerr << typ << " is not a placer type." << std::endl;
            abort();
        }
    }

    std::vector<unsigned> random_place(device::Device &device)
    {
        std::vector<unsigned> assign;
        for (unsigned i = 0; i < device.get_num_qubits(); ++i)
        {
            assign.push_back(i);
        }
        unsigned seed = std::chrono::system_clock::now().time_since_epoch().count();

        shuffle(assign.begin(), assign.end(), std::default_random_engine(seed));
        return assign;
    }

    std::vector<unsigned> static_place(device::Device &device)
    {
        std::vector<unsigned> assign;
        for (unsigned i = 0; i < device.get_num_qubits(); ++i)
        {
            assign.push_back(i);
        }
        return assign;
    }

    std::vector<unsigned> dfs_place(device::Device &device)
    {
        std::vector<unsigned> assign;
        std::vector<bool> qubit_mark(device.get_num_qubits(), false);
        dfs_device(0, device, assign, qubit_mark);
        assert(assign.size() == device.get_num_qubits());
        return assign;
    }

private:
    inline void dfs_device(unsigned current, device::Device &device, std::vector<unsigned> &assign, std::vector<bool> &qubit_mark)
    {
        if (qubit_mark[current] == true)
        {
            std::cout << current << std::endl;
        }
        assert(qubit_mark[current] == false);
        qubit_mark[current] = true;
        assign.push_back(current);

        device::Qubit &q = device.get_qubit(current);
        std::vector<unsigned> adj_waitlist;

        for (unsigned i = 0; i < q.get_adj_list().size(); ++i)
        {
            unsigned adj = q.get_adj_list()[i];

            // already marked
            if (qubit_mark[adj])
            {
                continue;
            }

            device::Qubit &adj_q = device.get_qubit(adj);
            assert(adj_q.get_adj_list().size() > 0);

            // corner
            if (adj_q.get_adj_list().size() == 1)
            {
                dfs_device(adj, device, assign, qubit_mark);
            }
            else
            {
                adj_waitlist.push_back(adj);
            }
        }
        for (unsigned i = 0; i < adj_waitlist.size(); ++i)
        {
            unsigned adj = adj_waitlist[i];
            if (qubit_mark[adj])
            {
                continue;
            }
            dfs_device(adj, device, assign, qubit_mark);
        }
        return;
    }

    std::vector<std::string> _placer_types;
};

class QFTRouter
{
public:
    QFTRouter(device::Device &device, std::string &typ) : _device(device)
    {
        if (typ == "naive")
        {
            _orient = false;
        }
        else if (typ == "orientation")
        {
            _orient = true;
        }
        else
        {
            std::cerr << typ << " is not a router type" << std::endl;
            abort();
        }
        _topo2device.resize(device.get_num_qubits());
        for (unsigned i = 0; i < device.get_num_qubits(); ++i)
        {
            _topo2device[device.get_qubit(i).get_topo_qubit()] = i;
        }
    }
    QFTRouter(const QFTRouter &other) = delete;
    QFTRouter(QFTRouter &&other) : _orient(other._orient), _device(other._device), _topo2device(std::move(other._topo2device)) {}

    unsigned get_gate_cost(topo::Gate &gate) const
    {
        std::tuple<unsigned, unsigned> device_qubits_idx = get_device_qubits_idx(gate);
        device::Qubit &q0 = _device.get_qubit(std::get<0>(device_qubits_idx));
        device::Qubit &q1 = _device.get_qubit(std::get<1>(device_qubits_idx));
        return std::max(q0.get_avail_time(), q1.get_avail_time());
    }

    void assign_gate(topo::Gate &gate)
    {
        std::tuple<unsigned, unsigned> device_qubits_idx = get_device_qubits_idx(gate);
        std::vector<unsigned> change_list = std::move(_device.routing(gate.get_type(), device_qubits_idx, _orient));
#ifdef DEBUG
        std::vector<bool> checker(_topo2device.size(), false);      
#endif
        for (unsigned i = 0; i < change_list.size(); ++i) // i is the idx of device qubit
        {
            unsigned topo_qubit_id = change_list[i];
            if (topo_qubit_id == UINT_MAX)
            {
                continue;
            }
#ifdef DEBUG
            assert(checker[i] == false);
            checker[i] = true;
#endif
            _topo2device[topo_qubit_id] = i;
        }
#ifdef DEBUG
        for (unsigned i = 0; i < checker.size(); ++i)
        {
            assert(checker[i]);
        }
        std::cout << "Gate: Q" << std::get<0>(gate.get_qubits()) << " Q" << std::get<1>(gate.get_qubits()) << "\n";
        _device.print_device_state(std::cout);
#endif
    }

private:
    std::tuple<unsigned, unsigned> get_device_qubits_idx(topo::Gate &gate) const
    {
        unsigned topo_idx_q0 = std::get<0>(gate.get_qubits()); // get operation qubit index of gate in topology
        unsigned device_idx_q0 = _topo2device[topo_idx_q0];    //get device qubit index of the gate

        unsigned topo_idx_q1 = std::get<1>(gate.get_qubits()); // get operation qubit index of gate in topology
        unsigned device_idx_q1 = _topo2device[topo_idx_q1];    //get device qubit index of the gate

        return std::make_tuple(device_idx_q0, device_idx_q1);
    }

    bool _orient;
    device::Device &_device;
    std::vector<unsigned> _topo2device;
};

class QFTScheduler
{
public:
    QFTScheduler(topo::Topology &topo) : _topo(topo) {}
    QFTScheduler(const QFTScheduler &other) = delete;
    QFTScheduler(QFTScheduler &&other) : _topo(other._topo) {}

    void assign_gates(device::Device &device, QFTRouter &router, std::string &typ)
    {
        if (typ == "random")
        {
            return assign_gates_random(device, router);
        }
        else if (typ == "static")
        {
            return assign_gates_static(device, router);
        }
        else if (typ == "greedy")
        {
            return assign_gates_greedy(device, router);
        }
        else if (typ == "old")
        {
            return assign_gates_old(device, router);
        }
        else
        {
            std::cerr << typ << " is not a scheduler type" << std::endl;
            abort();
        }
    }

    void assign_gates_random(device::Device &device, QFTRouter &router)
    {
#ifdef DEBUG
        unsigned count = 0;
#endif
        while (!_topo.get_avail_gates().empty())
        {
            std::vector<unsigned> &wait_list = _topo.get_avail_gates();
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

    void assign_gates_static(device::Device &device, QFTRouter &router)
    {
#ifdef DEBUG
        unsigned count = 0;
#endif
        while (!_topo.get_avail_gates().empty())
        {
            std::vector<unsigned> &wait_list = _topo.get_avail_gates();
            assert(wait_list.size() > 0);
            topo::Gate &gate = _topo.get_gate(wait_list[0]);
            router.assign_gate(gate);
#ifdef DEBUG
            count++;
#endif
            _topo.update_avail_gates(wait_list[0]);
        }
#ifdef DEBUG
        assert(count == _topo.get_num_gates());
#endif
    }

    void assign_gates_old(device::Device &device, QFTRouter &router)
    {
        for (unsigned i = 0; i < _topo.get_num_gates(); ++i)
        {
            topo::Gate &gate = _topo.get_gate(i);
            router.assign_gate(gate);
            _topo.update_avail_gates(i);
        }
    }

    void assign_gates_greedy(device::Device &device, QFTRouter &router)
    {
    }

private:
    topo::Topology &_topo;
};