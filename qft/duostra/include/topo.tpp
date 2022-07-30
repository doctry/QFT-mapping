namespace topo {

template <bool first>
vector<size_t> Topology::get_gates() const {
    using namespace std;
    vector<size_t> result;

    for (size_t i = 0; i < get_num_gates(); ++i) {
        const Gate& gate = get_gate(i);

        bool condition = first ? gate.is_first() : gate.is_last();
        if (condition) {
            result.push_back(i);
        }
    }

    return result;
}

template <bool first>
unordered_map<size_t, size_t> Topology::dist_to() const {
    using namespace std;

    unordered_map<size_t, size_t> dist;
    unordered_set<size_t> waiting;

    auto no_preceding = [&](size_t gate_idx) -> bool {
        const auto& gate = get_gate(gate_idx);

        if (first) {
            for (auto [idx, _] : gate.get_prevs()) {
                if (idx == ERROR_CODE) {
                    continue;
                }
                if (dist.find(idx) == dist.end()) {
                    return false;
                }
            }
            return true;
        } else {
            for (auto next : gate.get_nexts()) {
                if (next == ERROR_CODE) {
                    continue;
                }
                if (dist.find(next) == dist.end()) {
                    return false;
                }
            }
            return true;
        }
    };

    auto zero_nodes = get_gates<first>();
    for (auto node : zero_nodes) {
        waiting.insert(node);
    }

    size_t counter = 0;
    for (Tqdm bar{get_num_gates()}; waiting.size() != 0; ++counter) {
        auto cloned_waiting{waiting};

        vector<size_t> visited_this_cycle;
        for (size_t idx : cloned_waiting) {
            if (no_preceding(idx)) {
                const auto& gate = get_gate(idx);

                if (first) {
                    for (auto next : gate.get_nexts()) {
                        waiting.insert(next);
                    }
                } else {
                    for (auto prev : gate.get_prevs()) {
                        waiting.insert(prev.first);
                    }
                }

                visited_this_cycle.push_back(idx);
                waiting.erase(idx);
            }
        }

        for (auto vtc : visited_this_cycle) {
            dist[vtc] = counter;
            bar.add();
        }
    }

    return dist;
}

}  // namespace topo
