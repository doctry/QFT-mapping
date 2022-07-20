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
#include "util.hpp"

namespace topo {
using namespace std;

class Gate {
   public:
    Gate(unsigned id, Operator type, tuple<unsigned, unsigned> qs)
        : id_(id), type_(type), qubits_(qs) {
        prevs_.clear();
        nexts_.clear();
    }

    Gate(const Gate& other)
        : id_(other.id_),
          type_(other.type_),
          qubits_(other.qubits_),
          prevs_(other.prevs_),
          nexts_(other.nexts_) {}

    Gate(Gate&& other)
        : id_(other.id_),
          type_(other.type_),
          qubits_(other.qubits_),
          prevs_(move(other.prevs_)),
          nexts_(move(other.nexts_)) {}

    unsigned get_id() const { return id_; }
    tuple<unsigned, unsigned> get_qubits() const { return qubits_; }

    void set_type(Operator t) { type_ = t; }
    void set_prev(unsigned a, unsigned b);

    void add_next(unsigned n) {
        if (n != UINT_MAX) {
            nexts_.push_back(n);
        }
    }
    void finished(unsigned);
    bool is_avail() const;
    bool is_first() const;
    bool is_last() const;

    const vector<pair<unsigned, bool>>& get_prevs() const { return prevs_; }
    const vector<unsigned>& get_nexts() const { return nexts_; }
    Operator get_type() const { return type_; }

   private:
    unsigned id_;
    Operator type_;
    tuple<unsigned, unsigned> qubits_;
    vector<pair<unsigned, bool>> prevs_;
    vector<unsigned> nexts_;
};
}  // namespace topo
