#pragma once

#include "qft_topo.h"
#include "device.h"
#include <tuple>
#include <iostream>

class QFTMapper
{
public:
    QFTMapper(topo::QFTTopology &qft_topo, device::Device &device);
    QFTMapper(const QFTMapper &other) = delete;
    QFTMapper(QFTMapper &&other);

    void init_place();
    void assign_gates();

private:
    void assign_gate(topo::Gate &gate);
    std::tuple<unsigned, unsigned> get_device_qubits_idx(topo::Gate &gate);

    topo::QFTTopology &_qft_topo;
    device::Device &_device;
    std::vector<Operation> _ops;
};