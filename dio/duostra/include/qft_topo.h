#pragma once

#include <vector>
#include <tuple>
#include <limits.h>
#include <assert.h>

namespace topo
{
    class Qubit
    {
    public:
        Qubit(const unsigned i);
        Qubit(const Qubit &other) = delete;
        Qubit(Qubit &&other);

        void set_location(const unsigned i);
        const unsigned get_location() const;
    private:
        unsigned _id;
        unsigned _location;
    };

    class Gate
    {
    public:
        Gate(std::tuple<unsigned, unsigned> qs);
        Gate(const Gate &other) = delete;
        Gate(Gate &&other);

        const std::tuple<unsigned, unsigned> get_qubits() const;
    private:
        std::tuple<unsigned, unsigned> _qubits;
    };

    class QFTTopology
    {
    public:
        QFTTopology(unsigned num);
        QFTTopology(const QFTTopology &other) = delete;
        QFTTopology(QFTTopology &&other);

        const unsigned get_num_qubits() const;
        const unsigned get_num_gates() const;
        Gate &pop_next_gate();
        Gate &get_next_gate();
        Qubit &get_qubit(const unsigned i);

    private:
        std::vector<Gate> _gates;
        unsigned _next_gate_idx;
        std::vector<Qubit> _qubits;
    };
}