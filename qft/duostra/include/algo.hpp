#pragma once

#include <assert.h>
#include <algorithm>
#include <string>
#include <tuple>
#include <vector>
#include "topo.hpp"

namespace topo {
class AlgoTopology : public Topology {
  public:
    AlgoTopology(std::fstream &f, bool IBMGate) { parse(f, IBMGate); }
    AlgoTopology(const AlgoTopology &other) = delete;
    AlgoTopology(AlgoTopology&& other) {
        _gates = std::move(other._gates);
        _avail_gates = std::move(other._avail_gates);
    }

    ~AlgoTopology() {}

    void parse(std::fstream &, bool IBMGate);

    unsigned get_num_qubits() const override { return _num_qubits; }
    unsigned get_num_gates() const override { return _gates.size(); }
    Gate& get_gate(unsigned i) override { return _gates[i]; }

    const std::vector<unsigned>& get_avail_gates() const override {
        return _avail_gates;
    }

    void update_avail_gates(unsigned) override;
    void print_gates_with_next();
    void print_gates_with_prev();

   private:
    std::vector<unsigned> _last_gate;
};
};  // namespace topo
