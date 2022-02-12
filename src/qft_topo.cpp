#include "qft_topo.h"

topo::Qubit::Qubit(const unsigned i) : _id(i) {}
topo::Qubit::Qubit(Qubit &&other) : _id(other._id), _location(other._location) {}

topo::Gate::Gate(const unsigned q0, const unsigned q1) : _q0(q0), _q1(q1) {}
topo::Gate::Gate(Gate &&other) : _q0(other._q0), _q1(other._q1) {}

topo::QFTTopology::QFTTopology(unsigned num) : _next_gate_idx(0)
{
    for (unsigned i = 1; i < num; ++i)
    {
        Qubit qubit(i);
        _qubits.push_back(std::move(qubit));
        for (unsigned j = 0; j < i; ++j)
        {
            topo::Gate gate(j, i);
            _gates.push_back(std::move(gate));
        }
    }
}

topo::QFTTopology::QFTTopology(QFTTopology &&other) : _gates(std::move(other._gates)), _next_gate_idx(other._next_gate_idx) {}

const unsigned topo::QFTTopology::get_num_qubits() const{
    return _qubits.size();
}

topo::Gate &topo::QFTTopology::get_next_gate()
{
    return _gates[_next_gate_idx];
}

topo::Gate &topo::QFTTopology::pop_next_gate()
{
    topo::Gate &gate = get_next_gate();
    _next_gate_idx += 1;
    return gate;
}

topo::Qubit &topo::QFTTopology::get_qubit(const unsigned i)
{
    return _qubits[i];
}