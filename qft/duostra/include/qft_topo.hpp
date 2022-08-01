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
    QFTTopology(size_t num) noexcept;
    QFTTopology(const QFTTopology& other) noexcept;
    QFTTopology(QFTTopology&& other) noexcept;
    ~QFTTopology() override {}

    size_t get_num_qubits() const override { return num_qubits_; }
    size_t get_num_gates() const override { return gates_.size(); }
    const Gate& get_gate(size_t i) const { return gates_[i]; }
    Gate& get_gate(size_t i) override { return gates_[i]; }
    const std::vector<size_t>& get_avail_gates() const override {
        return avail_gates_;
    }
    unique_ptr<Topology> clone() const override {
        return make_unique<QFTTopology>(*this);
    }

    void update_avail_gates(size_t executed) override;

    void print_gates() const {
        std::vector<size_t> prevs;
        for (size_t i = 0; i < gates_.size(); ++i) {
            const Gate& gate = gates_[i];
            const std::vector<std::pair<size_t, bool>>& g_prevs =
                gate.get_prevs();
            const std::vector<size_t>& nexts = gate.get_nexts();
            std::vector<size_t> prev;
            for (size_t j = 0; j < g_prevs.size(); ++j) {
                prev.push_back(g_prevs[j].first);
            }
            std::cout << "gate[" << i << "]: prev: " << prev
                      << "next: " << nexts << "\n";
        }
    }
};
};  // namespace topo
