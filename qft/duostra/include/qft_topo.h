#pragma once

#include <vector>
#include <tuple>
#include <limits.h>
#include <assert.h>
#include <algorithm>

namespace topo
{
    class Gate
    {
    public:
        Gate(unsigned id, std::tuple<unsigned, unsigned> qs, unsigned pu, unsigned pl, unsigned nd, unsigned nr) : _id(id), _qubits(qs), _prev_up(pu), _prev_left(pl), _next_down(nd), _next_right(nr), _prev_up_mark(false), _prev_left_mark(false)
        {
            if (_prev_up == UINT_MAX)
            {
                _prev_up_mark = true;
            }
            if (_prev_left == UINT_MAX)
            {
                _prev_left_mark = true;
            }
        }
        Gate(const Gate &other) = delete;
        Gate(Gate &&other) : _id(other._id), _qubits(other._qubits), _prev_up(other._prev_up), _prev_left(other._prev_left), _next_down(other._next_down), _next_right(other._next_right), _prev_up_mark(other._prev_up_mark), _prev_left_mark(other._prev_left_mark) {}

        unsigned get_id() const
        {
            return _id;
        }

        std::vector<unsigned> get_nexts() const
        {
            std::vector<unsigned> ret;
            if (_next_down != UINT_MAX)
            {
                ret.push_back(_next_down);
            }
            if (_next_right != UINT_MAX)
            {
                ret.push_back(_next_right);
            }
            return ret;
        }

        bool is_avail(unsigned gate_id)
        {
            assert(gate_id == _prev_left || gate_id == _prev_up);
            if (gate_id == _prev_left)
            {
                assert(_prev_left_mark == false);
                _prev_left_mark = true;
            }
            if (gate_id == _prev_up)
            {
                assert(_prev_up_mark == false);
                _prev_up_mark = true;
            }
            return _prev_left_mark && _prev_up_mark;
        }

    private:
        unsigned _id;
        std::tuple<unsigned, unsigned> _qubits;
        unsigned _prev_up, _prev_left, _next_down, _next_right;
        bool _prev_up_mark, _prev_left_mark;
    };

    class QFTTopology
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
                    _gates.push_back(std::move(Gate(count, std::make_tuple(j, i), prev_up, prev_left, next_down, next_right)));
                    ++count;
                }
            }
        }
        QFTTopology(const QFTTopology &other) = delete;
        QFTTopology(QFTTopology &&other) : _gates(std::move(other._gates)), _avail_gates(other._avail_gates)
        {
        }

        const unsigned get_num_qubits() const
        {
            return _num;
        }
        const unsigned get_num_gates() const
        {
            return _gates.size();
        }
        Gate &get_gate(const unsigned i)
        {
            return _gates[i];
        }
        std::vector<unsigned>& get_avail_gates()
        {
            return _avail_gates;
        }

        void update_avail_gates(unsigned executed)
        {
            assert(std::find(_avail_gates.begin(), _avail_gates.end(), executed) != _avail_gates.end());
            Gate &g_exec = _gates[executed];
            _avail_gates.erase(std::remove(_avail_gates.begin(), _avail_gates.end(), executed), _avail_gates.end());
            assert(g_exec.get_id() == executed);

            std::vector<unsigned> nexts = g_exec.get_nexts();

            for (unsigned i = 0; i < nexts.size(); ++i)
            {
                unsigned n = nexts[i];
                if (_gates[n].is_avail(executed))
                {
                    _avail_gates.push_back(n);
                }
            }
        }

    private:
        unsigned _num;
        std::vector<Gate> _gates;
        std::vector<unsigned> _avail_gates;
    };
};