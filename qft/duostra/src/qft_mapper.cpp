#include "qft_mapper.hpp"

namespace scheduler {

 unique_ptr<Base> get(const string& typ,
                            unique_ptr<Topology> topo,
                            json& conf) {
    if (typ == "random") {
        return make_unique<Random>(move(topo));
    } else if (typ == "onion") {
        return make_unique<Onion>(move(topo), conf);
    } else if (typ == "static") {
        return make_unique<Static>(move(topo));
    } else if (typ == "greedy") {
        return make_unique<Greedy>(move(topo), conf);
    } else if (typ == "dora" || typ == "ChiangKaiShek" || typ == "cks") {
        return make_unique<Dora>(move(topo), conf);
    } else if (typ == "old") {
        return make_unique<Base>(move(topo));
    } else {
        cerr << typ << " is not a scheduler type" << endl;
        abort();
    }
}

}  // namespace scheduler
