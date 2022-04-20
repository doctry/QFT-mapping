#pragma once

#include "qft_topo.h"
#include "device.h"
#include <tuple>
#include <iostream>
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

    void place(device::Device &device, bool rand = false)
    {
        std::vector<unsigned> assign;
        for (unsigned i = 0; i < device.get_num_qubits(); ++i)
        {
            assign.push_back(i);
        }
        if (rand)
        {
            unsigned seed = std::chrono::system_clock::now().time_since_epoch().count();

            shuffle(assign.begin(), assign.end(), std::default_random_engine(seed));
        }

        for (unsigned i = 0; i < device.get_num_qubits(); ++i)
        {
            device::Qubit &device_qubit = device.get_qubit(i);
            device_qubit.set_topo_qubit(assign[i]);
        }
    }
};

class QFTRouter
{
public:
    QFTRouter(device::Device &device) : _device(device)
    {
        _topo2device.resize(device.get_num_qubits());
        for (unsigned i = 0; i < device.get_num_qubits(); ++i)
        {
            _topo2device[device.get_qubit(i).get_topo_qubit()] = i;
        }
    }
    QFTRouter(const QFTRouter &other) = delete;
    QFTRouter(QFTRouter &&other) : _device(other._device), _topo2device(std::move(other._topo2device)) {}

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
        std::vector<unsigned> change_list = std::move(_device.routing(device_qubits_idx));
        for (unsigned i = 0; i < change_list.size(); ++i) // i is the idx of device qubit
        {
            unsigned topo_qubit_id = change_list[i];
            _topo2device[topo_qubit_id] = i;
        }
#ifdef DEBUG
        std::cout << "Gate: Q" << std::get<0>(gate.get_qubits()) << " Q" << std::get<1>(gate.get_qubits()) << "\n";
        _device.print_device_state(std::cout);
#endif
    }

    std::vector<Operation> get_operations()
    {
        std::sort(_ops.begin(), _ops.end(), op_order);
        return _ops;
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

    device::Device &_device;
    std::vector<unsigned> _topo2device;
    std::vector<Operation> _ops;
};

class QFTScheduler
{
public:
    QFTScheduler(topo::QFTTopology &qft_topo): _qft_topo(qft_topo) {}
    QFTScheduler(const QFTScheduler &other) = delete;
    QFTScheduler(QFTScheduler &&other): _qft_topo(other._qft_topo) {}

    void assign_gates(device::Device &device, QFTRouter &router)
    {
        for (unsigned i = 0; i < _qft_topo.get_num_gates(); ++i)
        {
            topo::Gate &gate = _qft_topo.pop_next_gate();
            router.assign_gate(gate);
        }
    }

private:
    topo::QFTTopology &_qft_topo;
};