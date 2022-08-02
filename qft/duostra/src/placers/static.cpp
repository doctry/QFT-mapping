#include "qft_placer.hpp"

#include <random>

using namespace placer;
using namespace std;

vector<size_t> Static::place(Device& device) const {
    std::vector<size_t> assign;
    for (size_t i = 0; i < device.get_num_qubits(); ++i) {
        assign.push_back(i);
    }
    return assign;
}
