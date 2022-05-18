#pragma once

#include <vector>
#include <tuple>
#include "operator.h"

namespace topo
{
    class Gate
    {
    public:
        Gate(unsigned id, Operator type, std::tuple<unsigned, unsigned> qs) : _id(id), _type(type), _qubits(qs)
        {
            _prevs.clear();
            _nexts.clear();
        }
        Gate(const Gate &other) = delete;
        Gate(Gate &&other) : _id(other._id), _type(other._type), _qubits(other._qubits), _prevs(std::move(other._prevs)), _nexts(std::move(other._nexts)) {}

        unsigned get_id() const { return _id; }
        std::tuple<unsigned, unsigned> get_qubits() const { return _qubits; }

        void set_prev(unsigned a, unsigned b)
        {
            std::pair<unsigned, bool> first(a, false);
            std::pair<unsigned, bool> second(b, false);
            if (a != unsigned(-1))
                _prevs.push_back(first);
            if (a != b)
            {
                if (b != unsigned(-1))
                    _prevs.push_back(second);
            }
        }

        void add_next(unsigned n)
        {
            if (n != unsigned(-1))
                _nexts.push_back(n);
        }
        void finished(unsigned);
        bool is_avail();

        const std::vector<std::pair<unsigned, bool>> &get_prevs() const { return _prevs; }
        const std::vector<unsigned> &get_nexts() const { return _nexts; }
        const Operator get_type() const { return _type; }

    private:
        unsigned _id;
        Operator _type;
        std::tuple<unsigned, unsigned> _qubits;
        std::vector<std::pair<unsigned, bool>> _prevs;
        std::vector<unsigned> _nexts;
    };

    class Topology
    {
    public:
        virtual const unsigned get_num_qubits() const = 0;
        virtual const unsigned get_num_gates() const = 0;
        virtual Gate &get_gate(const unsigned i) = 0;
        virtual std::vector<unsigned> &get_avail_gates() = 0;
        virtual void update_avail_gates(unsigned executed) = 0;
    };
};