#include "qft_mapper.hpp"

namespace scheduler {

void Base::assign_gates(std::unique_ptr<QFTRouter> router) {
    std::cout << "Default scheduler running..." << std::endl;

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

void Random::assign_gates(std::unique_ptr<QFTRouter> router) {
    std::cout << "Random scheduler running..." << std::endl;

#ifdef DEBUG
    unsigned count = 0;
#endif

    for (Tqdm bar{topo_->get_num_gates()}; !topo_->get_avail_gates().empty();
         bar.add()) {
        auto& wait_list = topo_->get_avail_gates();
        assert(wait_list.size() > 0);
#ifndef DEBUG
        srand(std::chrono::system_clock::now().time_since_epoch().count());
#endif
        unsigned choose = rand() % wait_list.size();
        topo::Gate& gate = topo_->get_gate(wait_list[choose]);
        auto ops = router->assign_gate(gate);
        ops_.insert(ops_.end(), ops.begin(),
                    ops.end());  // append operations to the back
#ifdef DEBUG
        std::cout << wait_list << " " << wait_list[choose] << "\n\n";
        count++;
#endif
        topo_->update_avail_gates(wait_list[choose]);
    }
#ifdef DEBUG
    assert(count == topo_->get_num_gates());
#endif
}

void Static::assign_gates(std::unique_ptr<QFTRouter> router) {
    std::cout << "Static scheduler running..." << std::endl;

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

void Onion::assign_gates(std::unique_ptr<QFTRouter> router) {
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

void Greedy::assign_gates(std::unique_ptr<QFTRouter> router) {
    std::cout << "Greedy scheduler running..." << std::endl;

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
            std::vector<unsigned> cost_list(wait_list.size(), 0);
            for (unsigned i = 0; i < wait_list.size(); ++i) {
                topo::Gate& gate = topo_->get_gate(wait_list[i]);
                unsigned cost = router->get_gate_cost(gate, _conf.avail_typ,
                                                      _conf.apsp_coef);
                cost_list[i] = cost;
            }
            auto list_idx =
                _conf.cost_typ
                    ? std::max_element(cost_list.begin(), cost_list.end()) -
                          cost_list.begin()
                    : std::min_element(cost_list.begin(), cost_list.end()) -
                          cost_list.begin();
            gate_idx = wait_list[list_idx];
        }
        topo::Gate& gate = topo_->get_gate(gate_idx);
        auto ops = router->assign_gate(gate);
        ops_.insert(ops_.end(), ops.begin(),
                    ops.end());  // append operations to the back
#ifdef DEBUG
        std::cout << "waitlist: " << wait_list << " " << gate_idx << "\n\n";
        count++;
#endif
        topo_->update_avail_gates(gate_idx);
    }
#ifdef DEBUG
    assert(count == topo_->get_num_gates());
#endif
}

void BlockadeRevitalizer::assign_gates(std::unique_ptr<QFTRouter> router) {
    topo::DAG dag = topo_->dag();
    // generate all topo sorts under a particular depth

    // apply those routes
    // execute assign_gate interatively
}

void BlockadeRevitalizer::assign_one_gate(QFTRouter router) {}

std::unique_ptr<Base> get(const std::string& typ,
                          std::unique_ptr<topo::Topology>&& topo,
                          json& conf) {
    if (typ == "random") {
        return std::make_unique<Random>(std::move(topo));
    } else if (typ == "onion") {
        return std::make_unique<Onion>(std::move(topo), conf);
    } else if (typ == "static") {
        return std::make_unique<Static>(std::move(topo));
    } else if (typ == "greedy") {
        return std::make_unique<Greedy>(std::move(topo), conf);
    } else if (typ == "blkr") {
        return std::make_unique<BlockadeRevitalizer>(std::move(topo), conf);
    } else if (typ == "old") {
        return std::make_unique<Base>(std::move(topo));
    } else {
        std::cerr << typ << " is not a scheduler type" << std::endl;
        abort();
    }
}

}  // namespace scheduler
