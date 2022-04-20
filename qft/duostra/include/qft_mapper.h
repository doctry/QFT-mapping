#pragma once

#include "qft_topo.h"
#include "device.h"
#include <tuple>
#include <iostream>
#include <algorithm>
#include <vector>
#include <random>       // std::default_random_engine
#include <chrono>       // std::chrono::system_clock

class QFTPlacer
{
public:
    QFTPlacer() {}
    QFTPlacer(const QFTPlacer& other) = delete;
    QFTPlacer(QFTPlacer &&other) {}

    void place(device::Device &device, bool rand = false)
    {
        std::vector<unsigned> assign;
        for (unsigned i = 0; i < device.get_num_qubits(); ++i) {
            assign.push_back(i);
        }
        if (rand) {
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
    QFTRouter(topo::QFTTopology &qft_topo, device::Device &device) : _device(device) {}
    QFTRouter(const QFTRouter &other) = delete;
    QFTRouter(QFTRouter &&other) _qft_topo(other._qft_topo), _device(other._device) {}

    unsigned get_gate_cost(topo::Gate &gate) const
    {
        std::tuple<unsigned, unsigned> device_qubits_idx = get_device_qubits_idx(gate);
        device::Qubit& q0 = _device.get_qubit(std::get<0>(device_qubits_idx));
        device::Qubit& q1 = _device.get_qubit(std::get<1>(device_qubits_idx));
        return std::max(q0.get_avail_time(), q1.get_avail_time());
    }
    void assign_gate(topo::Gate &gate);

    std::vector<Operation> get_operations() {
        std::sort(_ops.begin(), _ops.end(), op_order);
        return _ops;
    }

private:
    std::tuple<unsigned, unsigned> get_device_qubits_idx(topo::Gate &gate) const;

    device::Device &_device;
    std::vector<Operation> _ops;
};

class QFTScheduler
{
public:
    QFTScheduler(topo::QFTTopology &qft_topo);
    QFTScheduler(const QFTScheduler &other) = delete;
    QFTScheduler(QFTScheduler &&other);

    void assign_gates(device::Device &device, QFTRouter& router) {
        for (unsigned i = 0; i < _qft_topo.get_num_gates(); ++i)
        {
            topo::Gate &gate = _qft_topo.pop_next_gate();
            router.assign_gate(gate);
        }
    }
    
private:

    topo::QFTTopology &_qft_topo;
};