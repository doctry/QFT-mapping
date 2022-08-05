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
        : id_(id),
          type_(type),
          qubits_(qs),
          prevs_(make_shared<vector<size_t>>()),
          nexts_(make_shared<vector<size_t>>()),
          prevs_ok_({}) {}

    Gate(const Gate& other)
        : id_(other.id_),
          type_(other.type_),
          qubits_(other.qubits_),
          prevs_(make_shared<vector<size_t>>(*other.prevs_)),
          nexts_(make_shared<vector<size_t>>(*other.nexts_)),
          prevs_ok_(other.prevs_ok_) {}

    Gate(Gate&& other)
        : id_(other.id_),
          type_(other.type_),
          qubits_(other.qubits_),
          prevs_(other.prevs_),
          nexts_(other.nexts_),
          prevs_ok_(move(other.prevs_ok_)) {}

    size_t get_id() const { return id_; }
    tuple<size_t, size_t> get_qubits() const { return qubits_; }

    void set_type(Operator t) { type_ = t; }

    void set_prev(size_t a, size_t b) {
        add_prev(a);
        add_prev(b);
    }
    void add_prev(size_t p) {
        if (p != ERROR_CODE) {
            prevs_->push_back(p);
            prevs_ok_.push_back(false);
        }
    }

    void add_next(size_t n) {
        if (n != ERROR_CODE) {
            nexts_->push_back(n);
        }
    }

    void mark_finished(size_t);
    bool is_avail() const {
        return all_of(prevs_ok_.begin(), prevs_ok_.end(),
                      [](bool ok) { return ok; });
    }
    bool is_first() const {
        return all_of(prevs_->begin(), prevs_->end(),
                      [](size_t idx) { return idx == ERROR_CODE; });
    }
    bool is_last() const {
        return all_of(nexts_->begin(), nexts_->end(),
                      [](size_t idx) { return idx == ERROR_CODE; });
    }

    const vector<size_t>& get_prevs() const { return *prevs_; }
    const vector<size_t>& get_nexts() const { return *nexts_; }
    const vector<bool>& get_prevs_ok() const { return prevs_ok_; }
    Operator get_type() const { return type_; }

   private:
    size_t id_;
    Operator type_;
    tuple<size_t, size_t> qubits_;

    // Since prevs and nexts are mostly constant (except in parse)
    // they can be shared between instances
    shared_ptr<vector<size_t>> prevs_;
    shared_ptr<vector<size_t>> nexts_;

    vector<bool> prevs_ok_;
};
}  // namespace topo
