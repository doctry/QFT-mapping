#pragma once

#include <assert.h>
#include <limits.h>
#include <algorithm>
#include <tuple>
#include <vector>
#include "topo.hpp"
#include "util.hpp"

namespace topo {
class QFTTopology : public Topology {
   public:
    QFTTopology(unsigned num) noexcept;
    QFTTopology(const QFTTopology& other) noexcept;
    QFTTopology(QFTTopology&& other) noexcept;
    ~QFTTopology() override {}

    unsigned get_num_qubits() const override { return num_qubits_; }
    unsigned get_num_gates() const override { return gates_.size(); }
    const Gate& get_gate(unsigned i) const { return gates_[i]; }
    Gate& get_gate(unsigned i) override { return gates_[i]; }
    const std::vector<unsigned>& get_avail_gates() const override {
        return avail_gates_;
    }
    unique_ptr<Topology> clone() const override {
        return make_unique<QFTTopology>(*this);
    }

    void update_avail_gates(unsigned executed) override;

    void print_gates() const {
        std::vector<unsigned> prevs;
        for (unsigned i = 0; i < gates_.size(); ++i) {
            const Gate& gate = gates_[i];
            const std::vector<std::pair<unsigned, bool>>& g_prevs =
                gate.get_prevs();
            const std::vector<unsigned>& nexts = gate.get_nexts();
            std::vector<unsigned> prev;
            for (unsigned j = 0; j < g_prevs.size(); ++j) {
                prev.push_back(g_prevs[j].first);
            }
            std::cout << "gate[" << i << "]: prev: " << prev
                      << "next: " << nexts << "\n";
        }
    }
};
};  // namespace topo
