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
#include "tqdm_wrapper.hpp"
#include "util.hpp"

namespace topo {
using namespace std;

class Gate {
   public:
    Gate(size_t id, Operator type, tuple<size_t, size_t> qs)
        : id_(id), type_(type), qubits_(qs), prevs_({}), nexts_({}) {}

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

    size_t get_id() const { return id_; }
    tuple<size_t, size_t> get_qubits() const { return qubits_; }

    void set_type(Operator t) { type_ = t; }
    void set_prev(size_t a, size_t b);

    void add_next(size_t n) {
        if (n != ERROR_CODE) {
            nexts_.push_back(n);
        }
    }
    void finished(size_t);
    bool is_avail() const;
    bool is_first() const;
    bool is_last() const;

    const vector<pair<size_t, bool>>& get_prevs() const { return prevs_; }
    const vector<size_t>& get_nexts() const { return nexts_; }
    Operator get_type() const { return type_; }

   private:
    size_t id_;
    Operator type_;
    tuple<size_t, size_t> qubits_;
    vector<pair<size_t, bool>> prevs_;
    vector<size_t> nexts_;
};
}  // namespace topo
