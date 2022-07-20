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
#include "gate.hpp"
#include "operator.hpp"
#include "tqdm.hpp"
#include "util.hpp"

namespace topo {
using namespace std;

class Topology {
   public:
    Topology() : num_qubits_(0), gates_({}), avail_gates_({}) {}

    Topology(const Topology& other)
        : num_qubits_(other.num_qubits_),
          gates_(other.gates_),
          avail_gates_(other.avail_gates_) {}

    Topology(Topology&& other)
        : num_qubits_(other.num_qubits_),
          gates_(std::move(other.gates_)),
          avail_gates_(std::move(other.avail_gates_)) {}

    virtual ~Topology() {}

    virtual size_t get_num_qubits() const = 0;
    virtual size_t get_num_gates() const = 0;
    virtual Gate& get_gate(size_t i) = 0;
    const Gate& get_gate(size_t i) const {
        return const_cast<topo::Topology*>(this)->get_gate(i);
    }
    virtual const vector<size_t>& get_avail_gates() const = 0;
    virtual void update_avail_gates(size_t executed) = 0;
    virtual unique_ptr<Topology> clone() const = 0;

    template <bool first>
    vector<size_t> get_gates() const;

    vector<size_t> get_first_gates() const { return get_gates<true>(); }
    vector<size_t> get_last_gates() const { return get_gates<false>(); }

    template <bool first>
    unordered_map<size_t, size_t> dist_to() const;
    unordered_map<size_t, size_t> dist_to_first() const {
        return dist_to<true>();
    }
    unordered_map<size_t, size_t> dist_to_last() const {
        return dist_to<false>();
    }

    unordered_map<size_t, vector<size_t>> gate_by_dist_to_first() const {
        auto dist = dist_to_first();
        std::cout << "Dist to first done\n";
        return gate_by_generation(dist);
    }
    unordered_map<size_t, vector<size_t>> gate_by_dist_to_last() const {
        auto dist = dist_to_last();
        std::cout << "Dist to last done\n";
        return gate_by_generation(dist);
    }

   protected:
    size_t num_qubits_;
    vector<Gate> gates_;
    vector<size_t> avail_gates_;

    static unordered_map<size_t, vector<size_t>> gate_by_generation(
        const unordered_map<size_t, size_t>& map);
};
};  // namespace topo

#include "topo.tpp"
