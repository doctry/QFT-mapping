#include "qft_placer.hpp"

#include <memory>
#include <string>

namespace placer {
unique_ptr<Base> get(const string& typ) {
    if (typ == "static") {
        return make_unique<Static>();
    } else if (typ == "random") {
        return make_unique<Random>();
    } else if (typ == "dfs") {
        return make_unique<DFS>();
    }
    cerr << typ << " is not a placer type." << endl;
    abort();
}
}  // namespace placer
