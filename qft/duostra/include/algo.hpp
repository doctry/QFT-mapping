#pragma once

#include <assert.h>
#include <algorithm>
#include <memory>
#include <string>
#include <tuple>
#include <vector>
#include "gate.hpp"
#include "topo.hpp"

namespace topo {
using namespace std;

class AlgoTopology : public Topology {
   public:
    AlgoTopology(std::fstream& f, bool IBMGate) : last_gate_({}) {
        parse(f, IBMGate);
    }
    AlgoTopology(const AlgoTopology& other) : Topology(other), last_gate_({}) {}
    AlgoTopology(AlgoTopology&& other)
        : Topology(std::move(other)), last_gate_({}) {}

    ~AlgoTopology() {}

    void parse(std::fstream&, bool IBMGate);

    size_t get_num_qubits() const override { return num_qubits_; }
    size_t get_num_gates() const override { return gates_.size(); }
    Gate& get_gate(size_t i) override { return gates_[i]; }
    unique_ptr<Topology> clone() const override {
        return make_unique<AlgoTopology>(*this);
    }

    const std::vector<size_t>& get_avail_gates() const override {
        return avail_gates_;
    }

    void update_avail_gates(size_t) override;
    void print_gates_with_next();
    void print_gates_with_prev();

   private:
    std::vector<size_t> last_gate_;
};

};  // namespace topo
