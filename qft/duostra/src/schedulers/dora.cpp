#include "qft_scheduler.hpp"

#include <vector>

using namespace std;
using namespace scheduler;

struct DoraCostTreeNode {
    vector<DoraCostTreeNode> children;
};

struct DoraCostTree {
    DoraCostTreeNode root;
};

void scheduler::Dora::assign_gates(unique_ptr<QFTRouter> router) {
    auto num_gates = topo_->get_num_gates();
    Tqdm bar{num_gates};
    for (size_t idx = 0; idx < num_gates; ++idx, bar.add()) {
        // generate all topo sorts under a particular depth
        auto paths_and_costs =
            paths_costs(depth, {}, topo_->clone(), router->clone());

        // apply those routes
        auto min =
            min_element(paths_and_costs.begin(), paths_and_costs.end(),
                        [](auto a, auto b) -> bool { return a.cost < b.cost; });
        auto min_cost = min->cost;
        auto min_path = min->path;
        (void)min_cost;

        assert(min_path.size() != 0);

        // execute assign_gate interatively
        route_gates(*router, min_path[0]);
    }
}

vector<PathsCosts> Dora::paths_costs(size_t depth,
                                     const vector<size_t>& path_so_far,
                                     unique_ptr<Topology> topo,
                                     unique_ptr<QFTRouter> router) const {
    if (!depth) {
        return {};
    }

    const auto& avail_gates = topo->get_avail_gates();

    vector<PathsCosts> paths;

    size_t cost_so_far = ops_cost();

    for (unsigned idx : avail_gates) {
        auto cloned_topo = topo->clone();
        auto cloned_router = router->clone();

        auto& gate = cloned_topo->get_gate(idx);
        router->assign_gate(gate);

        vector<size_t> current_path{path_so_far};
        current_path.push_back(idx);

        auto path_and_costs = paths_costs(
            depth - 1, current_path, move(cloned_topo), move(cloned_router));

        for (auto& cp : path_and_costs) {
            cp.cost += cost_so_far;
        }

        paths.insert(paths.end(), path_and_costs.begin(), path_and_costs.end());
    }

    return paths;
}
