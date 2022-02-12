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
        unsigned get_avail_time() const;

    private:
        unsigned _id;
        std::vector<unsigned> _adj_list;
        unsigned _topo_qubit;
        unsigned _occupied_until;
    };

    class Device
    {
    public:
        Device(std::fstream &file);
        Device(const Qubit &other) = delete;
        Device(Device &&other);

        const unsigned get_num_qubits() const;
        Qubit &get_qubit(const unsigned i);
    protected:
        std::vector<Qubit> _qubits;
    };
}