#pragma once

#include <cassert>
#include <climits>
#include <cmath>
#include <iostream>
#include <set>
#include <tuple>
#include <unordered_map>
#include <unordered_set>
#include <vector>
#include "operator.hpp"
#include "tqdm.hpp"

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

class DAGNode {
   public:
    DAGNode() : parents_({}), children_({}) {}

    DAGNode(set<size_t> parents, set<size_t> children)
        : parents_(parents), children_(children) {}

    set<size_t>& parents() { return parents_; }
    const set<size_t>& parents() const { return parents_; }

    set<size_t>& children() { return children_; }
    const set<size_t>& children() const { return children_; }

   private:
    set<size_t> parents_;
    set<size_t> children_;
};

class DAG {
   public:
    DAG(size_t num_nodes) {
        for (size_t idx = 0; idx < num_nodes; ++idx) {
            nodes_[idx] = DAGNode();
            heads_.insert(idx);
        }
    }

    size_t num_nodes() const { return nodes_.size(); }

    unordered_map<size_t, DAGNode>& nodes() { return nodes_; }
    const unordered_map<size_t, DAGNode>& nodes() const { return nodes_; }
    DAGNode& nodes(size_t idx) { return nodes_.at(idx); }
    const DAGNode& nodes(size_t idx) const { return nodes_.at(idx); }

    void link(size_t parent, size_t child);
    size_t remove(size_t idx);

    const unordered_set<size_t>& heads() const { return heads_; }

   private:
    unordered_map<size_t, DAGNode> nodes_;
    unordered_set<size_t> heads_;
};

class Topology {
   public:
    Topology() : _num_qubits(0), _gates({}), _avail_gates({}) {}

    Topology(const Topology& other)
        : _num_qubits(other._num_qubits),
          _gates(other._gates),
          _avail_gates(other._avail_gates) {}

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
    virtual unique_ptr<Topology> clone() const = 0;

    template <bool first>
    vector<unsigned> get_gates() const {
        using namespace std;
        vector<unsigned> result;

        for (unsigned i = 0; i < get_num_gates(); ++i) {
            const Gate& gate = get_gate(i);

            bool condition = first ? gate.is_first() : gate.is_last();
            if (condition) {
                result.push_back(i);
            }
        }

        return result;
    }

    vector<unsigned> get_first_gates() const { return get_gates<true>(); }
    vector<unsigned> get_last_gates() const { return get_gates<false>(); }

    template <bool first>
    unordered_map<unsigned, unsigned> dist_to() const {
        using namespace std;

        unordered_map<unsigned, unsigned> dist;
        unordered_set<unsigned> waiting;

        auto no_preceding = [&](size_t gate_idx) -> bool {
            const auto& gate = get_gate(gate_idx);

            if (first) {
                for (auto prev : gate.get_prevs()) {
                    if (prev.first == UINT_MAX) {
                        continue;
                    }
                    if (dist.find(prev.first) == dist.end()) {
                        return false;
                    }
                }
                return true;
            } else {
                for (auto next : gate.get_nexts()) {
                    if (next == UINT_MAX) {
                        continue;
                    }
                    if (dist.find(next) == dist.end()) {
                        return false;
                    }
                }
                return true;
            }
        };

        auto zero_nodes = get_gates<first>();
        for (auto node : zero_nodes) {
            waiting.insert(node);
        }

        unsigned counter = 0;
        for (Tqdm bar{get_num_gates()}; waiting.size() != 0;
             ++counter, bar.add()) {
            auto cloned_waiting{waiting};
            for (unsigned idx : cloned_waiting) {
                if (no_preceding(idx)) {
                    const auto& gate = get_gate(idx);

                    if (first) {
                        for (auto next : gate.get_nexts()) {
                            waiting.insert(next);
                        }
                    } else {
                        for (auto prev : gate.get_prevs()) {
                            waiting.insert(prev.first);
                        }
                    }

                    dist[idx] = counter;
                    waiting.erase(idx);
                }
            }
        }

        return dist;
    }

    unordered_map<unsigned, unsigned> dist_to_first() const {
        return dist_to<true>();
    }
    unordered_map<unsigned, unsigned> dist_to_last() const {
        return dist_to<false>();
    }

    unordered_map<unsigned, vector<unsigned>> gate_by_dist_to_first() const {
        auto dist = dist_to_first();
        std::cout << "Dist to first done\n";
        return gate_by_generation(dist);
    }
    unordered_map<unsigned, vector<unsigned>> gate_by_dist_to_last() const {
        auto dist = dist_to_last();
        std::cout << "Dist to last done\n";
        return gate_by_generation(dist);
    }

    DAG dag() const;

   protected:
    unsigned _num_qubits;
    vector<Gate> _gates;
    vector<unsigned> _avail_gates;

    static unordered_map<unsigned, vector<unsigned>> gate_by_generation(
        const unordered_map<unsigned, unsigned>& map);
};
};  // namespace topo
