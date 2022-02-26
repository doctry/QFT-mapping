#include "device.h"
#include <assert.h>
#include <algorithm>

Operation::Operation(Operator oper, std::tuple<unsigned, unsigned> qs) : _oper(oper), _qubits(qs)
{
}

Operation::Operation(Operation &&other) : _oper(other._oper), _qubits(other._qubits)
{
}

device::Qubit::Qubit(const unsigned i) : _id(i), _occupied_until(0) {}
device::Qubit::Qubit(Qubit &&other) : _id(other._id), _adj_list(std::move(other._adj_list)), _occupied_until(other._occupied_until) {}
void device::Qubit::add_adj(unsigned i)
{
    _adj_list.push_back(i);
}

const unsigned device::Qubit::get_avail_time() const
{
    return _occupied_until;
}

const bool device::Qubit::is_adj(Qubit &other) const
{
    return !(std::find(_adj_list.begin(), _adj_list.end(), other._id) == _adj_list.end());
}

void device::Qubit::set_topo_qubit(const unsigned i)
{
    _topo_qubit = i;
}

void device::Qubit::set_occupied_time(const unsigned t)
{
    _occupied_until = t;
}

device::Device::Device(std::fstream &file)
{
    unsigned num;
    file >> num;

    for (unsigned i = 0; i < num; ++i)
    {
        unsigned id;
        file >> id;
        assert(i == id);
        unsigned num_adj;
        file >> num_adj;

        device::Qubit qubit(i);
        for (unsigned j = 0; j < num_adj; ++j)
        {
            file >> id;
            qubit.add_adj(id);
        }

        _qubits.push_back(std::move(qubit));
    }
}
device::Device::Device(Device &&other) : _qubits(std::move(other._qubits)) {}

const unsigned device::Device::get_num_qubits() const
{
    return _qubits.size();
}

device::Qubit &device::Device::get_qubit(const unsigned i)
{
    return _qubits[i];
}

std::vector<Operation> device::Device::routing(std::tuple<unsigned, unsigned> qs)
{
    std::vector<Operation> ops;

    device::Qubit &q0 = get_qubit(std::get<0>(qs));
    device::Qubit &q1 = get_qubit(std::get<0>(qs));

    while (! q0.is_adj(q1)) {

    }
}