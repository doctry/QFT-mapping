#include "qft_scheduler.hpp"
#include "util.hpp"

using namespace scheduler;

Static::Static(unique_ptr<topo::Topology> topo) : Base(move(topo)) {}
Static::Static(const Static& other) : Base(other) {}
Static::Static(Static&& other) : Base(move(other)) {}

void Static::assign_gates(unique_ptr<QFTRouter> router) {
    cout << "Static scheduler running..." << endl;

    [[maybe_unused]] size_t count = 0;

    for (TqdmWrapper bar{topo_->get_num_gates()}; !bar.done(); ++bar) {
        auto& wait_list = topo_->get_avail_gates();
        assert(wait_list.size() > 0);

        size_t gate_idx = get_executable(*router);
        if (gate_idx == ERROR_CODE) {
            gate_idx = wait_list[0];
        }

        route_one_gate(*router, gate_idx);

        ++count;
    }
    assert(count == topo_->get_num_gates());
}

unique_ptr<Base> Static::clone() const {
    return make_unique<Static>(*this);
}
