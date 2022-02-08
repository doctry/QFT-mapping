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
    };

    class Gate
    {
    public:
        Gate(const unsigned i);
        Gate(const Gate &other) = delete;
        Gate(Gate &&other);

    private:
        unsigned _q0;
        unsigned _q1;
        unsigned _next_gate;
    };

    class DependencyGraph
    {
    public:
        DependencyGraph();
        DependencyGraph(const DependencyGraph &other) = delete;
        DependencyGraph(DependencyGraph &&other);
    };
}