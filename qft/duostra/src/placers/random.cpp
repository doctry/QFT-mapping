#include "qft_placer.hpp"

#include <random>

using namespace placer;
using namespace std;

vector<size_t> Random::place(Device& device) const {
    std::vector<size_t> assign;
    for (size_t i = 0; i < device.get_num_qubits(); ++i) {
        assign.push_back(i);
    }
    size_t seed = std::chrono::system_clock::now().time_since_epoch().count();

    shuffle(assign.begin(), assign.end(), std::default_random_engine(seed));
    return assign;
}
