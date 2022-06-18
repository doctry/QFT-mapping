#pragma once

#include "topo.hpp"
#include <algorithm>
#include <assert.h>
#include <string>
#include <tuple>
#include <vector>

namespace topo {
class AlgoTopology : public Topology {
  public:
    AlgoTopology(std::fstream &f) { parse(f); }
    AlgoTopology(const AlgoTopology &other) = delete;
    AlgoTopology(AlgoTopology &&other)
        : _gates(std::move(other._gates)),
          _avail_gates(std::move(other._avail_gates)) {}

    ~AlgoTopology() {}

    void parse(std::fstream &);

    unsigned get_num_qubits() const override { return _num; }
    unsigned get_num_gates() const override { return _gates.size(); }
    Gate &get_gate(unsigned i) override { return _gates[i]; }

    const std::vector<unsigned> &get_avail_gates() const override {
        return _avail_gates;
    }

    void update_avail_gates(unsigned) override;
    void print_gates_with_next();
    void print_gates_with_prev();

  private:
    unsigned _num;
    std::vector<Gate> _gates;
    std::vector<unsigned> _avail_gates;
    std::vector<unsigned> _last_gate;
};
}; // namespace topo
