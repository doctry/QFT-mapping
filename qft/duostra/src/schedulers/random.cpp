#include "qft_scheduler.hpp"

#include <chrono>

void scheduler::Random::assign_gates(unique_ptr<QFTRouter> router) {
    cout << "Random scheduler running..." << endl;

    // unsigned count = 0;

    for (Tqdm bar{topo_->get_num_gates()}; !topo_->get_avail_gates().empty();
         bar.add()) {
        auto& wait_list = topo_->get_avail_gates();
        assert(wait_list.size() > 0);
        srand(chrono::system_clock::now().time_since_epoch().count());

        unsigned choose = rand() % wait_list.size();

        route_gates(*router, wait_list[choose]);

        // cout << wait_list << " " << wait_list[choose] << "\n\n";
        // count++;
    }

    // assert(count == topo_->get_num_gates());
}
