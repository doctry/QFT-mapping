#include "qft_topo.h"

topo::Qubit::Qubit(const unsigned i) : _id(i), _location(UINT_MAX) {}
topo::Qubit::Qubit(Qubit &&other) : _id(other._id), _location(other._location) {}

void topo::Qubit::set_location(const unsigned i)
{
    assert(i < UINT_MAX);
    _location = i;
}
const unsigned topo::Qubit::get_location() const { return _location; }

topo::Gate::Gate(std::tuple<unsigned, unsigned> qs) : _qubits(qs) {}
topo::Gate::Gate(Gate &&other) : _qubits(other._qubits) {}

const std::tuple<unsigned, unsigned> topo::Gate::get_qubits() const { return _qubits; }

topo::QFTTopology::QFTTopology(unsigned num) : _next_gate_idx(0)
{
    for (unsigned i = 0; i < num; ++i)
    {
        Qubit qubit(i);
        _qubits.push_back(std::move(qubit));
        for (unsigned j = 0; j < i; ++j)
        {
            topo::Gate gate(std::make_tuple(j, i));
            _gates.push_back(std::move(gate));
        }
    }
}

topo::QFTTopology::QFTTopology(QFTTopology &&other) : _gates(std::move(other._gates)), _next_gate_idx(other._next_gate_idx) {}

const unsigned topo::QFTTopology::get_num_qubits() const
{
    return _qubits.size();
}

const unsigned topo::QFTTopology::get_num_gates() const
{
    return _gates.size();
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