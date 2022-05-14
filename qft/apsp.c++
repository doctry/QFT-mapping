#include "apsp.h++"
#include <iostream>

using namespace std;

torch::Tensor apsp(torch::Tensor adj_matrix) {
    cout << adj_matrix.sizes() << '\n';

    static_assert(false);
}
