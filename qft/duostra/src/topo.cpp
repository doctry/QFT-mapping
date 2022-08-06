#include "topo.hpp"
#include "qft_topo.hpp"
#include "util.hpp"

using namespace topo;
using namespace std;

void Topology::update_avail_gates(size_t executed) {
    assert(find(begin(avail_gates_), end(avail_gates_), executed) !=
           end(avail_gates_));
    const Gate& g_exec = get_gate(executed);
    avail_gates_.erase(remove(begin(avail_gates_), end(avail_gates_), executed),
                       end(avail_gates_));
    assert(g_exec.get_id() == executed);

    executed_gates_[executed] = 0;
    for (size_t next : g_exec.get_nexts()) {
        if (get_gate(next).is_avail(executed_gates_)) {
            avail_gates_.push_back(next);
        }
    }

    vector<size_t> gates_to_trim;
    for (size_t prev_id : g_exec.get_prevs()) {
        const auto& prev_gate = get_gate(prev_id);
        ++executed_gates_[prev_id];

        if (executed_gates_[prev_id] >= prev_gate.get_nexts().size()) {
            gates_to_trim.push_back(prev_id);
        }
    }

    for (size_t gate_id : gates_to_trim) {
        executed_gates_.erase(gate_id);
    }
}

QFTTopology::QFTTopology(size_t num) {
    size_t num_qubits = num;
    vector<Gate> all_gates;
    assert(num > 0);

    for (size_t i = 0, count = 0; i < num; ++i) {
        for (size_t j = 0; j < i; ++j, ++count) {
            size_t prev_up = (j == i - 1) ? ERROR_CODE : count + 1 - i;
            size_t prev_left = (j == 0) ? ERROR_CODE : count - 1;
            size_t next_down = (i == num - 1) ? ERROR_CODE : count + i;
            size_t next_right = (j == i - 1) ? ERROR_CODE : count + 1;

            Gate gate{count, Operator::CX, std::make_tuple(j, i)};
            gate.add_prev(prev_up);
            gate.add_prev(prev_left);
            gate.add_next(next_down);
            gate.add_next(next_right);

            all_gates.push_back(std::move(gate));
        }
    }
    avail_gates_.push_back(0);

    dep_graph_ = make_shared<DependencyGraph>(num_qubits, move(all_gates));
}

QFTTopology::QFTTopology(const QFTTopology& other) : Topology(other) {}
QFTTopology::QFTTopology(QFTTopology&& other) : Topology(std::move(other)) {}
