#include "qft_mapper.hpp"

void QFTScheduler::assign_gates(QFTRouter& router) {
    std::cout << "Default scheduler running..." << std::endl;

    Tqdm bar{_topo.get_num_gates()};
    for (unsigned i = 0; i < _topo.get_num_gates(); ++i) {
        bar.add();
        topo::Gate& gate = _topo.get_gate(i);
        std::vector<device::Operation> ops = router.assign_gate(gate);
        _ops.insert(_ops.end(), ops.begin(),
                    ops.end());  // append operations to the back
        _topo.update_avail_gates(i);
    }
}

void QFTSchedulerRandom::assign_gates(QFTRouter& router) {
std::cout << "Random scheduler running..." << std::endl;

#ifdef DEBUG
    unsigned count = 0;
#endif

    for (Tqdm bar{_topo.get_num_gates()}; !_topo.get_avail_gates().empty();
         bar.add()) {
        auto& wait_list = _topo.get_avail_gates();
        assert(wait_list.size() > 0);
#ifndef DEBUG
        srand(std::chrono::system_clock::now().time_since_epoch().count());
#endif
        unsigned choose = rand() % wait_list.size();
        topo::Gate& gate = _topo.get_gate(wait_list[choose]);
        std::vector<device::Operation> ops = router.assign_gate(gate);
        _ops.insert(_ops.end(), ops.begin(),
                    ops.end());  // append operations to the back
#ifdef DEBUG
        std::cout << wait_list << " " << wait_list[choose] << "\n\n";
        count++;
#endif
        _topo.update_avail_gates(wait_list[choose]);
    }
#ifdef DEBUG
    assert(count == _topo.get_num_gates());
#endif
}

void QFTSchedulerStatic::assign_gates(QFTRouter& router) {
    std::cout << "Static scheduler running..." << std::endl;

#ifdef DEBUG
    unsigned count = 0;
#endif

    for (Tqdm bar{_topo.get_num_gates()}; !_topo.get_avail_gates().empty();
         bar.add()) {
        auto& wait_list = _topo.get_avail_gates();
        assert(wait_list.size() > 0);
        unsigned gate_idx = get_executable(router, wait_list);
        if (gate_idx == UINT_MAX) {
            gate_idx = wait_list[0];
        }
        topo::Gate& gate = _topo.get_gate(gate_idx);
        std::vector<device::Operation> ops = router.assign_gate(gate);
        _ops.insert(_ops.end(), ops.begin(),
                    ops.end());  // append operations to the back
#ifdef DEBUG
        count++;
#endif
        _topo.update_avail_gates(gate_idx);
    }
#ifdef DEBUG
    assert(count == _topo.get_num_gates());
#endif
}

void QFTSchedulerDP::assign_gates(QFTRouter& router) {
    std::cout << "Dynamic scheduler running..." << std::endl;
    for (Tqdm bar{_topo.get_num_gates()}; !_topo.get_avail_gates().empty();
         bar.add()) {
    }
}

void QFTSchedulerGreedy::assign_gates(QFTRouter& router) {
    std::cout << "Greedy scheduler running..." << std::endl;

#ifdef DEBUG
    unsigned count = 0;
#endif
    auto topo_wrap = TopologyWrapperWithCandidate(_topo, _conf.candidates);

    for (Tqdm bar{_topo.get_num_gates()}; !topo_wrap.get_avail_gates().empty();
         bar.add()) {
        auto wait_list = topo_wrap.get_avail_gates();
        assert(wait_list.size() > 0);

        unsigned gate_idx = get_executable(router, wait_list);
        if (gate_idx == UINT_MAX) {
            std::vector<unsigned> cost_list(wait_list.size(), 0);
            for (unsigned i = 0; i < wait_list.size(); ++i) {
                topo::Gate& gate = _topo.get_gate(wait_list[i]);
                unsigned cost =
                    router.get_gate_cost(gate, _conf.min_max, _conf.apsp_coef);
                cost_list[i] = cost;
            }
            gate_idx =
                wait_list[std::min_element(cost_list.begin(), cost_list.end()) -
                          cost_list.begin()];
        }
        topo::Gate& gate = _topo.get_gate(gate_idx);
        std::vector<device::Operation> ops = router.assign_gate(gate);
        _ops.insert(_ops.end(), ops.begin(),
                    ops.end());  // append operations to the back
#ifdef DEBUG
        std::cout << "waitlist: " << wait_list << " " << gate_idx << "\n\n";
        count++;
#endif
        _topo.update_avail_gates(gate_idx);
    }
#ifdef DEBUG
    assert(count == _topo.get_num_gates());
#endif
}

std::unique_ptr<QFTScheduler> get_scheduler(std::string& typ, topo::Topology& topo, json& conf) {
    if (typ == "random") {
        return std::make_unique<QFTSchedulerRandom>(topo);
    } else if (typ == "dp") {
        return std::make_unique<QFTSchedulerDP>(topo);
    } else if (typ == "static") {
        return std::make_unique<QFTSchedulerStatic>(topo);
    } else if (typ == "greedy") {
        return std::make_unique<QFTSchedulerGreedy>(topo, conf);
    } else if (typ == "old") {
        return std::make_unique<QFTScheduler>(topo);
    } else {
        std::cerr << typ << " is not a scheduler type" << std::endl;
        abort();
    }
}