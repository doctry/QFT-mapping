#include "qft_scheduler.hpp"

using namespace scheduler;

Onion::Onion(unique_ptr<Topology> topo, json& conf)
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

    Tqdm bar{num_gates};

    // size_t total_size = 0;
    // for (const auto& gg : gen_to_gates) {
    //     total_size += gg.second.size();
    // }
    // assert(total_size == num_gates);

    while (gen_to_gates.size()) {
        using gen_pair = const pair<size_t, vector<size_t>>&;
        auto select =
            first_mode_
                ? [](gen_pair a, gen_pair b) { return a.first < b.first; }
                : [](gen_pair a, gen_pair b) { return a.first > b.first; };

        auto youngest =
            min_element(gen_to_gates.begin(), gen_to_gates.end(), select);

        auto distance = youngest->first;
        auto& wait_list = youngest->second;

        for (; wait_list.size() > 0; bar.add()) {
            size_t gate_idx = get_executable(*router, wait_list);
            gate_idx = greedy_fallback(*router, wait_list, gate_idx);

            auto erase_idx =
                std::find(wait_list.begin(), wait_list.end(), gate_idx);
            assert(erase_idx != wait_list.end());

            swap(*erase_idx, *wait_list.rbegin());
            wait_list.pop_back();

            // --total_size;
            route_one_gate(*router, gate_idx);
        }
        assert(youngest->second.empty());
        gen_to_gates.erase(youngest->first);
    }

    auto& avail_gates = topo_->get_avail_gates();
    assert(avail_gates.empty());

    // assert(total_size == 0);

    std::cout << avail_gates.size() << "\n";
}

unique_ptr<SchedulerBase> Onion::clone() const {
    return make_unique<Onion>(*this);
}
