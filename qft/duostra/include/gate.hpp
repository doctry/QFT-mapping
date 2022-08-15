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
        : id_(id), type_(type), qubits_(qs), prevs_({}), nexts_({}) {
        if (std::get<0>(qubits_) > std::get<1>(qubits_)) {
            qubits_ =
                std::make_tuple(std::get<1>(qubits_), std::get<0>(qubits_));
        }
    }

    Gate(const Gate& other) = delete;

    Gate(Gate&& other)
        : id_(other.id_),
          type_(other.type_),
          qubits_(other.qubits_),
          prevs_(other.prevs_),
          nexts_(other.nexts_) {}

    size_t get_id() const { return id_; }
    tuple<size_t, size_t> get_qubits() const { return qubits_; }

    void set_type(Operator t) { type_ = t; }

    void add_prev(size_t p) {
        if (p != ERROR_CODE) {
            prevs_.push_back(p);
        }
    }

    void add_next(size_t n) {
        if (n != ERROR_CODE) {
            nexts_.push_back(n);
        }
    }

    bool is_avail(const unordered_map<size_t, size_t>& executed_gates) const {
        return all_of(prevs_.begin(), prevs_.end(), [&](size_t prev) -> bool {
            return executed_gates.find(prev) != executed_gates.end();
        });
    }

    bool is_first() const { return prevs_.empty(); }
    bool is_last() const { return nexts_.empty(); }

    const vector<size_t>& get_prevs() const { return prevs_; }
    const vector<size_t>& get_nexts() const { return nexts_; }
    Operator get_type() const { return type_; }

   private:
    size_t id_;
    Operator type_;
    tuple<size_t, size_t> qubits_;
    vector<size_t> prevs_;
    vector<size_t> nexts_;
};
}  // namespace topo
