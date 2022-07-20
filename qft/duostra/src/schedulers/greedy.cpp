#include "qft_scheduler.hpp"

#include <vector>

class TopologyCandidate {
   public:
    TopologyCandidate(const topo::Topology& topo, size_t candidate)
        : topo_(topo), cands_(candidate) {}

    std::vector<unsigned> get_avail_gates() const {
        auto& gates = topo_.get_avail_gates();

        if (gates.size() < cands_) {
            return gates;
        }

        return std::vector<unsigned>(gates.begin(), gates.begin() + cands_);
    }

   private:
    const topo::Topology& topo_;
    size_t cands_;
};

scheduler::Greedy::Greedy(unique_ptr<topo::Topology>&& topo,
                          json& conf) noexcept
    : Base(move(topo)) {
    int candidates = json_get<int>(conf, "candidates");
    if (candidates > 0) {
        conf_.candidates = candidates;
    }
    conf_.apsp_coef = json_get<unsigned>(conf, "apsp_coef");
    string avail_typ = json_get<string>(conf, "avail");
    if (avail_typ == "min") {
        conf_.avail_typ = false;
    } else if (avail_typ == "max") {
        conf_.avail_typ = true;
    } else {
        cerr << "\"min_max\" can only be \"min\" or \"max\"." << endl;
        abort();
    }
    string cost_typ = json_get<string>(conf, "cost");
    if (cost_typ == "min") {
        conf_.cost_typ = false;
    } else if (cost_typ == "max") {
        conf_.cost_typ = true;
    } else {
        cerr << "\"min_max\" can only be \"min\" or \"max\"." << endl;
        abort();
    }
}

void scheduler::Greedy::assign_gates(unique_ptr<QFTRouter> router) {
    cout << "Greedy scheduler running..." << endl;

    // unsigned count = 0;
    auto topo_wrap = TopologyCandidate(*topo_, conf_.candidates);

    for (Tqdm bar{topo_->get_num_gates()}; !topo_wrap.get_avail_gates().empty();
         bar.add()) {
        auto wait_list = topo_wrap.get_avail_gates();
        assert(wait_list.size() > 0);

        unsigned gate_idx = get_executable(*router, wait_list);
        gate_idx = greedy_fallback(*router, wait_list, gate_idx);
        route_gates(*router, gate_idx);
        // cout << "waitlist: " << wait_list << " " << gate_idx << "\n\n";
        // count++;
    }
    // assert(count == topo_->get_num_gates());
}

unsigned scheduler::Greedy::greedy_fallback(
    const QFTRouter& router,
    const std::vector<unsigned>& wait_list,
    unsigned gate_idx) const {
    if (gate_idx != UINT_MAX) {
        return gate_idx;
    }
    vector<unsigned> cost_list(wait_list.size(), 0);

    for (unsigned i = 0; i < wait_list.size(); ++i) {
        topo::Gate& gate = topo_->get_gate(wait_list[i]);
        cost_list[i] =
            router.get_gate_cost(gate, conf_.avail_typ, conf_.apsp_coef);
    }

    auto list_idx = conf_.cost_typ
                        ? max_element(cost_list.begin(), cost_list.end()) -
                              cost_list.begin()
                        : min_element(cost_list.begin(), cost_list.end()) -
                              cost_list.begin();
    return wait_list[list_idx];
}
