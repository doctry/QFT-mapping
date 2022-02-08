#include <vector>
#include <fstream>

#pragma once

namespace device
{
    class Qubit
    {
    public:
        Qubit(const unsigned i);
        Qubit(const Qubit &other) = delete;
        Qubit(Qubit &&other);

        void add_adj(unsigned i);

    private:
        unsigned _id;
        std::vector<unsigned> _adj_list;
        unsigned _topo_qubit;
    };

    class Device
    {
    public:
        Device(std::fstream &file);
        Device(const Qubit &other) = delete;
        Device(Device &&other);

    protected:
        std::vector<Qubit> _qubits;
    };
}