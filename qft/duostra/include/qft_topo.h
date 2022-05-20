#pragma once

#include <vector>
#include <tuple>
#include <limits.h>
#include <assert.h>
#include <algorithm>
#include "topo.h"
#include "util.hpp"

namespace topo
{
    class QFTTopology : public Topology
    {
    public:
        QFTTopology(unsigned num) : _num(num)
        {
            assert(num > 0);

            unsigned count = 0;
            for (unsigned i = 0; i < num; ++i)
            {
                for (unsigned j = 0; j < i; ++j)
                {
                    unsigned prev_up = (j == i - 1) ? UINT_MAX : count + 1 - i;
                    unsigned prev_left = (j == 0) ? UINT_MAX : count - 1;
                    unsigned next_down = (i == num - 1) ? UINT_MAX : count + i;
                    unsigned next_right = (j == i - 1) ? UINT_MAX : count + 1;
                    Gate gate(count, Operator::R, std::make_tuple(j, i));
                    gate.set_prev(prev_up, prev_left);
                    gate.add_next(next_down);
                    gate.add_next(next_right);
                    _gates.push_back(std::move(gate));
                    ++count;
                }
            }
            _avail_gates.push_back(0);
        }
        QFTTopology(const QFTTopology &other) = delete;
        QFTTopology(QFTTopology &&other) : _gates(std::move(other._gates)), _avail_gates(other._avail_gates)
        {
        }

        const unsigned get_num_qubits() const override
        {
            return _num;
        }
        const unsigned get_num_gates() const override
        {
            return _gates.size();
        }
        Gate &get_gate(const unsigned i) override
        {
            return _gates[i];
        }
        std::vector<unsigned> &get_avail_gates() override
        {
            return _avail_gates;
        }

        void update_avail_gates(unsigned executed) override
        {
            assert(std::find(_avail_gates.begin(), _avail_gates.end(), executed) != _avail_gates.end());
            Gate &g_exec = _gates[executed];
            _avail_gates.erase(std::remove(_avail_gates.begin(), _avail_gates.end(), executed), _avail_gates.end());
            assert(g_exec.get_id() == executed);

            std::vector<unsigned> nexts = g_exec.get_nexts();

            for (unsigned i = 0; i < nexts.size(); ++i)
            {
                unsigned n = nexts[i];
                Gate &gate = _gates[n];
                gate.finished(executed);
                if (_gates[n].is_avail())
                {
                    _avail_gates.push_back(n);
                }
            }
        }

        inline void print_gates() const
        {
            #ifdef DEBUG
            std::vector<unsigned> prevs;
            for (unsigned i = 0; i < _gates.size(); ++i)
            {
                const Gate &gate = _gates[i];
                const std::vector<std::pair<unsigned, bool>> & g_prevs = gate.get_prevs();
                const std::vector<unsigned> &nexts = gate.get_nexts();
                std::vector<unsigned> prev;
                for (unsigned j = 0; j < g_prevs.size(); ++j)
                {
                    prev.push_back(g_prevs[j].first);
                }
                std::cout << "gate[" << i << "]: prev: " << prev << "next: " << nexts << "\n";
            }
            #endif
        }

    private:
        unsigned _num;
        std::vector<Gate> _gates;
        std::vector<unsigned> _avail_gates;
    };
};