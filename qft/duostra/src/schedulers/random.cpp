#include "qft_scheduler.hpp"

#include <chrono>

using namespace scheduler;

Random::Random(unique_ptr<topo::Topology> topo) noexcept
    : SchedulerBase(move(topo)) {}

Random::Random(const Random& other) noexcept : SchedulerBase(other) {}

Random::Random(Random&& other) noexcept : SchedulerBase(move(other)) {}

void Random::assign_gates(unique_ptr<QFTRouter> router) {
    cout << "Random scheduler running..." << endl;

    // unsigned count = 0;

    for (Tqdm bar{topo_->get_num_gates()}; !topo_->get_avail_gates().empty();
         bar.add()) {
        auto& wait_list = topo_->get_avail_gates();
        assert(wait_list.size() > 0);
        srand(chrono::system_clock::now().time_since_epoch().count());

        unsigned choose = rand() % wait_list.size();

        route_one_gate(*router, wait_list[choose]);

        // cout << wait_list << " " << wait_list[choose] << "\n\n";
        // count++;
    }

    // assert(count == topo_->get_num_gates());
}

unique_ptr<SchedulerBase> Random::clone() const {
    return make_unique<Random>(*this);
}
