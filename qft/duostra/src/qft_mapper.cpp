#include "qft_mapper.h"

void QFTMapper::assign_gates()
{
    for (unsigned i = 0; i < _qft_topo.get_num_gates(); ++i)
    {
        topo::Gate &gate = _qft_topo.pop_next_gate();
        assign_gate(gate);
    }
}

unsigned QFTMapper::get_gate_cost(topo::Gate &gate) const {
    std::tuple<unsigned, unsigned> device_qubits_idx = get_device_qubits_idx(gate);
    device::Qubit& q0 = _device.get_qubit(std::get<0>(device_qubits_idx));
    device::Qubit& q1 = _device.get_qubit(std::get<1>(device_qubits_idx));
    return std::max(q0.get_avail_time(), q1.get_avail_time());
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
    #ifdef DEBUG
    std::cout << "Gate: Q" << std::get<0>(gate.get_qubits()) << " Q" << std::get<1>(gate.get_qubits()) << "\n";
    _device.print_device_state(std::cout);
    #endif
}

std::tuple<unsigned, unsigned> QFTMapper::get_device_qubits_idx(topo::Gate &gate) const {
    unsigned topo_idx_q0 = std::get<0>(gate.get_qubits()); // get operation qubit index of gate in topology
    topo::Qubit& topo_q0 = _qft_topo.get_qubit(topo_idx_q0); // get operation qubit of gate in topology
    unsigned device_idx_q0 = topo_q0.get_location(); //get device qubit index of the gate

    unsigned topo_idx_q1 = std::get<1>(gate.get_qubits()); // get operation qubit index of gate in topology
    topo::Qubit& topo_q1 = _qft_topo.get_qubit(topo_idx_q1); // get operation qubit of gate in topology
    unsigned device_idx_q1 = topo_q1.get_location(); //get device qubit index of the gate

    return std::make_tuple(device_idx_q0, device_idx_q1);
}