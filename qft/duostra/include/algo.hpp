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
    AlgoTopology(std::fstream& f, bool IBMGate) { parse(f, IBMGate); }
    AlgoTopology(const AlgoTopology& other) : Topology(other) {}
    AlgoTopology(AlgoTopology&& other) : Topology(std::move(other)) {}

    ~AlgoTopology() {}

    void parse(std::fstream&, bool IBMGate);

    unsigned get_num_qubits() const override { return num_qubits_; }
    unsigned get_num_gates() const override { return gates_.size(); }
    Gate& get_gate(unsigned i) override { return gates_[i]; }
    unique_ptr<Topology> clone() const override {
        return make_unique<AlgoTopology>(*this);
    }

    const std::vector<unsigned>& get_avail_gates() const override {
        return avail_gates_;
    }

    void update_avail_gates(unsigned) override;
    void print_gates_with_next();
    void print_gates_with_prev();

   private:
    std::vector<unsigned> last_gate_;
};

};  // namespace topo
