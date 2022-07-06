#pragma once

#include <cassert>
#include <tuple>
#include <unordered_map>
#include <vector>
#include "operator.hpp"

namespace topo {
using namespace std;
class Gate {
   public:
    Gate(unsigned id, Operator type, tuple<unsigned, unsigned> qs)
        : _id(id), _type(type), _qubits(qs) {
        _prevs.clear();
        _nexts.clear();
    }

    Gate(const Gate& other)
        : _id(other._id),
          _type(other._type),
          _qubits(other._qubits),
          _prevs(other._prevs),
          _nexts(other._nexts) {}

    Gate(Gate&& other)
        : _id(other._id),
          _type(other._type),
          _qubits(other._qubits),
          _prevs(move(other._prevs)),
          _nexts(move(other._nexts)) {}

    unsigned get_id() const { return _id; }
    tuple<unsigned, unsigned> get_qubits() const { return _qubits; }

    void set_type(Operator t) { _type = t; }
    void set_prev(unsigned a, unsigned b) {
        pair<unsigned, bool> first(a, false);
        pair<unsigned, bool> second(b, false);

        if (a != (unsigned)-1) {
            _prevs.push_back(first);
        }

        if (a != b && b != (unsigned)-1) {
            _prevs.push_back(second);
        }
    }

    void add_next(unsigned n) {
        if (n != (unsigned)-1) {
            _nexts.push_back(n);
        }
    }
    void finished(unsigned);
    bool is_avail() const;

    bool is_first() const;
    bool is_last() const;

    const vector<pair<unsigned, bool>>& get_prevs() const { return _prevs; }
    const vector<unsigned>& get_nexts() const { return _nexts; }
    Operator get_type() const { return _type; }

   private:
    unsigned _id;
    Operator _type;
    tuple<unsigned, unsigned> _qubits;
    vector<pair<unsigned, bool>> _prevs;
    vector<unsigned> _nexts;
};

class Topology {
   public:
    Topology() : _num_qubits(0), _gates({}), _avail_gates({}) {}
    Topology(Topology&& other)
        : _num_qubits(other._num_qubits),
          _gates(std::move(other._gates)),
          _avail_gates(std::move(other._avail_gates)) {}

    virtual ~Topology() {}
    virtual unsigned get_num_qubits() const = 0;
    virtual unsigned get_num_gates() const = 0;
    virtual Gate& get_gate(unsigned i) = 0;
    const Gate& get_gate(unsigned i) const {
        return const_cast<topo::Topology*>(this)->get_gate(i);
    }
    virtual const vector<unsigned>& get_avail_gates() const = 0;
    virtual void update_avail_gates(unsigned executed) = 0;

    vector<unsigned> get_first_gates() const;
    vector<unsigned> get_last_gates() const;

    unordered_map<unsigned, unsigned> dist_to_first() const;
    unordered_map<unsigned, unsigned> dist_to_last() const;

    unordered_map<unsigned, vector<unsigned>> gate_by_dist_to_first() const;
    unordered_map<unsigned, vector<unsigned>> gate_by_dist_to_last() const;

   protected:
    unsigned _num_qubits;
    std::vector<Gate> _gates;
    std::vector<unsigned> _avail_gates;

    static unordered_map<unsigned, vector<unsigned>> gate_by_generation(
        const unordered_map<unsigned, unsigned>& map);
};
};  // namespace topo
