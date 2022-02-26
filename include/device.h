#include <vector>
#include <fstream>
#include <tuple>

#pragma once

enum Operator {
    Swap,
    R
};

class Operation
{
public:
    Operation(Operator oper, std::tuple<unsigned, unsigned> qs);
    Operation(const Operation &other) = delete;
    Operation(Operation &&other);

private:
    Operator _oper;
    std::tuple<unsigned, unsigned> _qubits;
};
namespace device
{
    class Qubit
    {
    public:
        Qubit(const unsigned i);
        Qubit(const Qubit &other) = delete;
        Qubit(Qubit &&other);

        void add_adj(unsigned i);
        const unsigned get_avail_time() const;
        const bool is_adj(Qubit& other) const;

        void set_topo_qubit(const unsigned i);
        void set_occupied_time(const unsigned t);

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
        std::vector<Operation> routing(std::tuple<unsigned, unsigned> qs);

    protected:
        std::vector<Qubit> _qubits;
    };
}