#include "qft_mapper.h"

QFTMapper::QFTMapper(topo::QFTTopology &qft_topo, device::Device &device) : _qft_topo(qft_topo), _device(device) {}
QFTMapper::QFTMapper(QFTMapper &&other) : _qft_topo(other._qft_topo), _device(other._device) {}

void QFTMapper::init_place()
{
    for (unsigned i = 0; i < _qft_topo.get_num_qubits(); ++i)
    {
        topo::Qubit &topo_qubit = _qft_topo.get_qubit(i);
        device::Qubit &device_qubit = _device.get_qubit(i);

        topo_qubit.set_location(i);
        device_qubit.set_topo_qubit(i);
    }
}

void QFTMapper::assign_gates()
{
    for (unsigned i = 0; i < _qft_topo.get_num_gates(); ++i)
    {
        topo::Gate &gate = _qft_topo.pop_next_gate();
        assign_gate(gate);
    }
}

void QFTMapper::assign_gate(topo::Gate &gate)
{
     std::tuple<unsigned, unsigned> device_qubits_idx = get_device_qubits_idx(gate);
     std::vector<unsigned> change_list = std::move(_device.routing(device_qubits_idx));
     for (unsigned i = 0; i < change_list.size(); ++i) // i is the idx of device qubit
     {
         unsigned topo_qubit_id = change_list[i];
         topo::Qubit& topo_qubit = _qft_topo.get_qubit(topo_qubit_id);
         topo_qubit.set_location(i);
     }
}

std::tuple<unsigned, unsigned> QFTMapper::get_device_qubits_idx(topo::Gate &gate) {
    unsigned topo_idx_q0 = std::get<0>(gate.get_qubits()); // get operation qubit index of gate in topology
    topo::Qubit& topo_q0 = _qft_topo.get_qubit(topo_idx_q0); // get operation qubit of gate in topology
    unsigned device_idx_q0 = topo_q0.get_location(); //get device qubit index of the gate

    unsigned topo_idx_q1 = std::get<1>(gate.get_qubits()); // get operation qubit index of gate in topology
    topo::Qubit& topo_q1 = _qft_topo.get_qubit(topo_idx_q0); // get operation qubit of gate in topology
    unsigned device_idx_q1 = topo_q0.get_location(); //get device qubit index of the gate

    return std::make_tuple(device_idx_q0, device_idx_q1);
}