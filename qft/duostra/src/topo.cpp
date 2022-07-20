#include "topo.hpp"
#include "qft_topo.hpp"

using namespace topo;

QFTTopology::QFTTopology(unsigned num) noexcept {
    num_qubits_ = num;
    assert(num > 0);

    for (unsigned i = 0, count = 0; i < num; ++i) {
        for (unsigned j = 0; j < i; ++j, ++count) {
            unsigned prev_up = (j == i - 1) ? UINT_MAX : count + 1 - i;
            unsigned prev_left = (j == 0) ? UINT_MAX : count - 1;
            unsigned next_down = (i == num - 1) ? UINT_MAX : count + i;
            unsigned next_right = (j == i - 1) ? UINT_MAX : count + 1;

            Gate gate{count, Operator::CX, std::make_tuple(j, i)};
            gate.set_prev(prev_up, prev_left);
            gate.add_next(next_down);
            gate.add_next(next_right);
            gates_.push_back(std::move(gate));
        }
    }
    avail_gates_.push_back(0);
}

QFTTopology::QFTTopology(const QFTTopology& other) noexcept : Topology(other) {}
QFTTopology::QFTTopology(QFTTopology&& other) noexcept
    : Topology(std::move(other)) {}

void QFTTopology::update_avail_gates(unsigned executed) {
    assert(std::find(avail_gates_.begin(), avail_gates_.end(), executed) !=
           avail_gates_.end());
    Gate& g_exec = gates_[executed];
    avail_gates_.erase(
        std::remove(avail_gates_.begin(), avail_gates_.end(), executed),
        avail_gates_.end());
    assert(g_exec.get_id() == executed);

    std::vector<unsigned> nexts = g_exec.get_nexts();

    for (unsigned i = 0; i < nexts.size(); ++i) {
        unsigned n = nexts[i];
        Gate& gate = gates_[n];
        gate.finished(executed);
        if (gates_[n].is_avail()) {
            avail_gates_.push_back(n);
        }
    }
}
