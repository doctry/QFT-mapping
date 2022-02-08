#include "device.h"
#include <assert.h>

device::Qubit::Qubit(const unsigned i) : _id(i) {}
device::Qubit::Qubit(Qubit &&other) : _id(other._id), _adj_list(std::move(other._adj_list)) {}
void device::Qubit::add_adj(unsigned i) {
    _adj_list.push_back(i);
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
