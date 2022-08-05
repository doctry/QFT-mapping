#include "topo.hpp"
#include "qft_topo.hpp"
#include "util.hpp"

using namespace topo;
using namespace std;

void Topology::update_avail_gates(size_t executed) {
    assert(find(begin(avail_gates_), end(avail_gates_), executed) !=
           end(avail_gates_));
    const Gate& g_exec = gates_[executed];
    avail_gates_.erase(remove(begin(avail_gates_), end(avail_gates_), executed),
                       end(avail_gates_));
    assert(g_exec.get_id() == executed);

    const auto &nexts{g_exec.get_nexts()};

    for (size_t i = 0; i < nexts.size(); ++i) {
        size_t n = nexts[i];
        gates_[n].mark_finished(executed);
        if (gates_[n].is_avail()) {
            avail_gates_.push_back(n);
        }
    }
}

QFTTopology::QFTTopology(size_t num) {
    num_qubits_ = num;
    assert(num > 0);

    for (size_t i = 0, count = 0; i < num; ++i) {
        for (size_t j = 0; j < i; ++j, ++count) {
            size_t prev_up = (j == i - 1) ? ERROR_CODE : count + 1 - i;
            size_t prev_left = (j == 0) ? ERROR_CODE : count - 1;
            size_t next_down = (i == num - 1) ? ERROR_CODE : count + i;
            size_t next_right = (j == i - 1) ? ERROR_CODE : count + 1;

            Gate gate{count, Operator::CX, std::make_tuple(j, i)};
            gate.set_prev(prev_up, prev_left);
            gate.add_next(next_down);
            gate.add_next(next_right);
            gates_.push_back(std::move(gate));
        }
    }
    avail_gates_.push_back(0);
}

QFTTopology::QFTTopology(const QFTTopology& other) : Topology(other) {}
QFTTopology::QFTTopology(QFTTopology&& other) : Topology(std::move(other)) {}
