namespace topo {

template <bool first>
vector<unsigned> Topology::get_gates() const {
    using namespace std;
    vector<unsigned> result;

    for (unsigned i = 0; i < get_num_gates(); ++i) {
        const Gate& gate = get_gate(i);

        bool condition = first ? gate.is_first() : gate.is_last();
        if (condition) {
            result.push_back(i);
        }
    }

    return result;
}

template <bool first>
unordered_map<unsigned, unsigned> Topology::dist_to() const {
    using namespace std;

    unordered_map<unsigned, unsigned> dist;
    unordered_set<unsigned> waiting;

    auto no_preceding = [&](size_t gate_idx) -> bool {
        const auto& gate = get_gate(gate_idx);

        if (first) {
            for (auto prev : gate.get_prevs()) {
                if (prev.first == UINT_MAX) {
                    continue;
                }
                if (dist.find(prev.first) == dist.end()) {
                    return false;
                }
            }
            return true;
        } else {
            for (auto next : gate.get_nexts()) {
                if (next == UINT_MAX) {
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

    unsigned counter = 0;
    for (Tqdm bar{get_num_gates()}; waiting.size() != 0; ++counter) {
        auto cloned_waiting{waiting};

        vector<unsigned> visited_this_cycle;
        for (unsigned idx : cloned_waiting) {
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
