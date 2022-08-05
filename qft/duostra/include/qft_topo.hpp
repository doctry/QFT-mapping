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
    QFTTopology(size_t num);
    QFTTopology(const QFTTopology& other);
    QFTTopology(QFTTopology&& other);
    ~QFTTopology() override {}

    unique_ptr<Topology> clone() const override {
        return make_unique<QFTTopology>(*this);
    }

    void print_gates() const {
        std::vector<size_t> prevs;
        const auto& gates = dep_graph_->gates();
        for (size_t i = 0; i < gates.size(); ++i) {
            const Gate& gate = gates[i];
            const auto& prevs = gate.get_prevs();
            const auto& nexts = gate.get_nexts();
            std::cout << "gate[" << i << "]: prev: " << prevs
                      << "next: " << nexts << "\n";
        }
    }
};
};  // namespace topo
