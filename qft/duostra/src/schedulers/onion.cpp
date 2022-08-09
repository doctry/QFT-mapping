#include "qft_scheduler.hpp"

using namespace scheduler;

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
            for (auto idx : gate.get_prevs()) {
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
    for (TqdmWrapper bar{get_num_gates()}; waiting.size() != 0; ++counter) {
        const auto cloned_waiting{waiting};

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
                        waiting.insert(prev);
                    }
                }

                visited_this_cycle.push_back(idx);
                waiting.erase(idx);
            }
        }

        for (auto vtc : visited_this_cycle) {
            dist[vtc] = counter;
            ++bar;
        }
    }

    return dist;
}

unordered_map<size_t, vector<size_t>> Topology::gate_by_dist_to_first() const {
    auto dist{dist_to_first()};
    std::cout << "Dist to first done\n";
    return gate_by_generation(dist);
}

unordered_map<size_t, vector<size_t>> Topology::gate_by_dist_to_last() const {
    auto dist{dist_to_last()};
    std::cout << "Dist to last done\n";
    return gate_by_generation(dist);
}

}  // namespace topo

Onion::Onion(unique_ptr<Topology> topo, const json& conf)
    : Greedy(move(topo), conf),
      first_mode_(json_get<bool>(conf, "layer_from_first")) {}

Onion::Onion(const Onion& other)
    : Greedy(other), first_mode_(other.first_mode_) {}

Onion::Onion(Onion&& other)
    : Greedy(move(other)), first_mode_(other.first_mode_) {}

void Onion::assign_gates(unique_ptr<QFTRouter> router) {
    using namespace std;
    cout << "Onion scheduler running..." << endl;

    auto gen_to_gates = first_mode_ ? topo_->gate_by_dist_to_first()
                                    : topo_->gate_by_dist_to_last();

    size_t num_gates = topo_->get_num_gates();

    TqdmWrapper bar{num_gates};

    // Used for debugging.
    size_t total_size = 0;
    for (const auto& gg : gen_to_gates) {
        total_size += gg.second.size();
    }
    assert(total_size == num_gates);

    while (gen_to_gates.size()) {
        assign_generation(*router, gen_to_gates, bar, total_size);
    }

    auto& avail_gates = topo_->get_avail_gates();
    assert(avail_gates.empty());

    assert(total_size == 0);

    std::cout << avail_gates.size() << "\n";
}

void Onion::assign_generation(
    QFTRouter& router,
    std::unordered_map<size_t, std::vector<size_t>>& gen_to_gates,
    TqdmWrapper& bar,
    size_t& total_size) {
    using gen_pair = const pair<size_t, vector<size_t>>&;
    auto select =
        first_mode_ ? [](gen_pair a, gen_pair b) { return a.first < b.first; }
                    : [](gen_pair a, gen_pair b) { return a.first > b.first; };

    auto youngest =
        min_element(gen_to_gates.begin(), gen_to_gates.end(), select);

    assert(youngest != gen_to_gates.end());

    // Distance is only for debugging use.
    auto& [distance, wait_list] = *youngest;
    // [[maybe_unused]] auto distance = youngest->first;
    // auto& wait_list = youngest->second;

    for (; wait_list.size() > 0; ++bar) {
        assign_from_wait_list(router, wait_list, total_size);
    }

    assert(wait_list.empty());
    gen_to_gates.erase(distance);
}

void Onion::assign_from_wait_list(QFTRouter& router,
                                  vector<size_t>& wait_list,
                                  size_t& total_size) {
    size_t gate_idx = get_executable(router, wait_list);
    gate_idx = greedy_fallback(router, wait_list, gate_idx);

    auto erase_idx = std::find(wait_list.begin(), wait_list.end(), gate_idx);
    assert(erase_idx != wait_list.end());

    swap(*erase_idx, *wait_list.rbegin());
    wait_list.pop_back();

    --total_size;
    route_one_gate(router, gate_idx);
}

unique_ptr<Base> Onion::clone() const {
    return make_unique<Onion>(*this);
}
