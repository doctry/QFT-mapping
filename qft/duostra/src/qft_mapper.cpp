#include "qft_mapper.hpp"

namespace scheduler {

void Base::assign_gates(unique_ptr<QFTRouter> router) {
    cout << "Default scheduler running..." << endl;

    Tqdm bar{topo_->get_num_gates()};
    for (unsigned i = 0; i < topo_->get_num_gates(); ++i) {
        bar.add();
        topo::Gate& gate = topo_->get_gate(i);
        auto ops = router->assign_gate(gate);
        ops_.insert(ops_.end(), ops.begin(),
                    ops.end());  // append operations to the back
        topo_->update_avail_gates(i);
    }
}

void Random::assign_gates(unique_ptr<QFTRouter> router) {
    cout << "Random scheduler running..." << endl;

#ifdef DEBUG
    unsigned count = 0;
#endif

    for (Tqdm bar{topo_->get_num_gates()}; !topo_->get_avail_gates().empty();
         bar.add()) {
        auto& wait_list = topo_->get_avail_gates();
        assert(wait_list.size() > 0);
#ifndef DEBUG
        srand(chrono::system_clock::now().time_since_epoch().count());
#endif
        unsigned choose = rand() % wait_list.size();
        topo::Gate& gate = topo_->get_gate(wait_list[choose]);
        auto ops = router->assign_gate(gate);
        ops_.insert(ops_.end(), ops.begin(),
                    ops.end());  // append operations to the back
#ifdef DEBUG
        cout << wait_list << " " << wait_list[choose] << "\n\n";
        count++;
#endif
        topo_->update_avail_gates(wait_list[choose]);
    }
#ifdef DEBUG
    assert(count == topo_->get_num_gates());
#endif
}

void Static::assign_gates(unique_ptr<QFTRouter> router) {
    cout << "Static scheduler running..." << endl;

#ifdef DEBUG
    unsigned count = 0;
#endif

    for (Tqdm bar{topo_->get_num_gates()}; !topo_->get_avail_gates().empty();
         bar.add()) {
        auto& wait_list = topo_->get_avail_gates();
        assert(wait_list.size() > 0);
        unsigned gate_idx = get_executable(*router, wait_list);
        if (gate_idx == UINT_MAX) {
            gate_idx = wait_list[0];
        }
        topo::Gate& gate = topo_->get_gate(gate_idx);
        auto ops = router->assign_gate(gate);
        ops_.insert(ops_.end(), ops.begin(),
                    ops.end());  // append operations to the back
#ifdef DEBUG
        count++;
#endif
        topo_->update_avail_gates(gate_idx);
    }
#ifdef DEBUG
    assert(count == topo_->get_num_gates());
#endif
}

void Onion::assign_gates(unique_ptr<QFTRouter> router) {
    using namespace std;
    cout << "Onion scheduler running..." << endl;

    auto gen_to_gates = first_mode_ ? topo_->gate_by_dist_to_first()
                                    : topo_->gate_by_dist_to_last();

    unsigned num_gates = topo_->get_num_gates();

    Tqdm bar{num_gates};
    while (gen_to_gates.size() != 0) {
        auto youngest =
            cost_typ_
                ? max_element(gen_to_gates.begin(), gen_to_gates.end(),
                              [](const pair<unsigned, vector<unsigned>>& a,
                                 const pair<unsigned, vector<unsigned>>& b) {
                                  return a.first < b.first;
                              })
                : min_element(gen_to_gates.begin(), gen_to_gates.end(),
                              [](const pair<unsigned, vector<unsigned>>& a,
                                 const pair<unsigned, vector<unsigned>>& b) {
                                  return a.first < b.first;
                              });

        for (auto idx : youngest->second) {
            bar.add();

            auto& gate = topo_->get_gate(idx);
            auto ops = router->assign_gate(gate);
            ops_.insert(ops_.end(), ops.begin(), ops.end());
        }

        gen_to_gates.erase(youngest->first);
    }

    assert(topo_->get_avail_gates().empty());
}

void Greedy::assign_gates(unique_ptr<QFTRouter> router) {
    cout << "Greedy scheduler running..." << endl;

#ifdef DEBUG
    unsigned count = 0;
#endif
    auto topo_wrap = TopologyWrapperWithCandidate(*topo_, _conf.candidates);

    for (Tqdm bar{topo_->get_num_gates()}; !topo_wrap.get_avail_gates().empty();
         bar.add()) {
        auto wait_list = topo_wrap.get_avail_gates();
        assert(wait_list.size() > 0);

        unsigned gate_idx = get_executable(*router, wait_list);
        if (gate_idx == UINT_MAX) {
            vector<unsigned> cost_list(wait_list.size(), 0);
            for (unsigned i = 0; i < wait_list.size(); ++i) {
                topo::Gate& gate = topo_->get_gate(wait_list[i]);
                unsigned cost = router->get_gate_cost(gate, _conf.avail_typ,
                                                      _conf.apsp_coef);
                cost_list[i] = cost;
            }
            auto list_idx =
                _conf.cost_typ
                    ? max_element(cost_list.begin(), cost_list.end()) -
                          cost_list.begin()
                    : min_element(cost_list.begin(), cost_list.end()) -
                          cost_list.begin();
            gate_idx = wait_list[list_idx];
        }
        topo::Gate& gate = topo_->get_gate(gate_idx);
        auto ops = router->assign_gate(gate);
        ops_.insert(ops_.end(), ops.begin(),
                    ops.end());  // append operations to the back
#ifdef DEBUG
        cout << "waitlist: " << wait_list << " " << gate_idx << "\n\n";
        count++;
#endif
        topo_->update_avail_gates(gate_idx);
    }
#ifdef DEBUG
    assert(count == topo_->get_num_gates());
#endif
}

void Dora::assign_gates(unique_ptr<QFTRouter> router) {
    auto num_gates = topo_->get_num_gates();
    Tqdm bar{num_gates};
    for (size_t idx = 0; idx < num_gates; ++idx, bar.add()) {
        // generate all topo sorts under a particular depth
        auto paths_and_costs =
            paths_costs(depth, topo_->clone(), router->clone());

        // apply those routes
        auto min = min_element(
            paths_and_costs.begin(), paths_and_costs.end(),
            [](auto a, auto b) -> bool { return a.first < b.first; });
        auto min_cost = min->first;
        auto min_path = min->second;

        assert(min_path.size() != 0);

        // execute assign_gate interatively
        auto& gate = topo_->get_gate(min_path[0]);
        auto ops = router->assign_gate(gate);
        ops_.insert(ops_.end(), ops.begin(), ops.end());
    }
}

vector<pair<size_t, vector<size_t>>> Dora::paths_costs(
    size_t depth,
    unique_ptr<Topology> topo,
    unique_ptr<QFTRouter> router) const {
    if (!depth) {
        return {};
    }

    const auto& avail_gates = topo->get_avail_gates();

    vector<pair<size_t, vector<size_t>>> paths;

    size_t cost_so_far = router->cost_so_far();

    for (unsigned idx : avail_gates) {
        auto cloned_topo = topo->clone();
        auto cloned_router = router->clone();

        auto& gate = cloned_topo->get_gate(idx);
        router->assign_gate(gate);

        auto path_and_costs =
            paths_costs(depth - 1, move(cloned_topo), move(cloned_router));

        for (auto& cp : path_and_costs) {
            cp.first += cost_so_far;
        }

        paths.insert(paths.end(), path_and_costs.begin(), path_and_costs.end());
    }

    return paths;
}

unique_ptr<Base> get(const string& typ,
                     unique_ptr<Topology>&& topo,
                     json& conf) {
    if (typ == "random") {
        return make_unique<Random>(move(topo));
    } else if (typ == "onion") {
        return make_unique<Onion>(move(topo), conf);
    } else if (typ == "static") {
        return make_unique<Static>(move(topo));
    } else if (typ == "greedy") {
        return make_unique<Greedy>(move(topo), conf);
    } else if (typ == "dora") {
        return make_unique<Dora>(move(topo), conf);
    } else if (typ == "old") {
        return make_unique<Base>(move(topo));
    } else {
        cerr << typ << " is not a scheduler type" << endl;
        abort();
    }
}

}  // namespace scheduler
