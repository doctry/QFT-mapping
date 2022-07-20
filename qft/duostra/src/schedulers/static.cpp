#include "qft_scheduler.hpp"

using namespace scheduler;

Static::Static(unique_ptr<topo::Topology> topo) noexcept
    : SchedulerBase(move(topo)) {}
Static::Static(const Static& other) noexcept : SchedulerBase(other) {}
Static::Static(Static&& other) noexcept : SchedulerBase(move(other)) {}

void Static::assign_gates(unique_ptr<QFTRouter> router) {
    cout << "Static scheduler running..." << endl;

    // unsigned count = 0;

    for (Tqdm bar{topo_->get_num_gates()}; !topo_->get_avail_gates().empty();
         bar.add()) {
        auto& wait_list = topo_->get_avail_gates();
        assert(wait_list.size() > 0);

        unsigned gate_idx = get_executable(*router, wait_list);
        if (gate_idx == UINT_MAX) {
            gate_idx = wait_list[0];
        }

        route_one_gate(*router, gate_idx);

        // count++;
    }
    // assert(count == topo_->get_num_gates());
}

unique_ptr<SchedulerBase> Static::clone() const {
    return make_unique<Static>(*this);
}