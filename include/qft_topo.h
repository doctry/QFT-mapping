#include <vector>

#pragma once

namespace topo
{
    class Qubit
    {
    public:
        Qubit(const unsigned i);
        Qubit(const Qubit &other) = delete;
        Qubit(Qubit &&other);

    private:
        unsigned _id;
        unsigned _location;
    };

    class Gate
    {
    public:
        Gate(const unsigned q0, const unsigned q1);
        Gate(const Gate &other) = delete;
        Gate(Gate &&other);

    private:
        unsigned _q0;
        unsigned _q1;
    };

    class QFTTopology
    {
    public:
        QFTTopology(unsigned num);
        QFTTopology(const QFTTopology &other) = delete;
        QFTTopology(QFTTopology &&other);

        const unsigned get_num_qubits() const;
        Gate &pop_next_gate();
        Gate &get_next_gate();
        Qubit &get_qubit(const unsigned i);

    private:
        std::vector<Gate> _gates;
        unsigned _next_gate_idx;
        std::vector<Qubit> _qubits;
    };
}