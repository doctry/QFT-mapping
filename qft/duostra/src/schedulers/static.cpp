#include "qft_scheduler.hpp"

using namespace scheduler;

Static::Static(unique_ptr<topo::Topology> topo) : SchedulerBase(move(topo)) {}
Static::Static(const Static& other) : SchedulerBase(other) {}
Static::Static(Static&& other) : SchedulerBase(move(other)) {}

void Static::assign_gates(unique_ptr<QFTRouter> router) {
    cout << "Static scheduler running..." << endl;

    [[maybe_unused]] size_t count = 0;

    for (Tqdm bar{topo_->get_num_gates()}; !topo_->get_avail_gates().empty();
         bar.add()) {
        auto& wait_list = topo_->get_avail_gates();
        assert(wait_list.size() > 0);

        size_t gate_idx = get_executable(*router, wait_list);
        if (gate_idx == size_t(-1)) {
            gate_idx = wait_list[0];
        }

        route_one_gate(*router, gate_idx);

        ++count;
    }
    assert(count == topo_->get_num_gates());
}

unique_ptr<SchedulerBase> Static::clone() const {
    return make_unique<Static>(*this);
}
