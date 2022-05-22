#include "apsp.hpp"

#include <cassert>
#include <cmath>
#include <iostream>
#include <utility>

using namespace std;
using namespace torch::indexing;

std::ostream &operator<<(std::ostream &out, const ShortestPath sp) {
    return out << "Cost:\n"
               << sp.cost << '\n'
               << "Pointer:\n"
               << sp.pointer << "\n\n";
}

static ShortestPath floyd_warshall(torch::Tensor adj_mat);

ShortestPath apsp(torch::Tensor adj_mat) {
    torch::NoGradGuard no_grad;

    return std::move(floyd_warshall(adj_mat));
}

static ShortestPath floyd_warshall(torch::Tensor adj_mat) {
    assert(adj_mat.sizes().size() == 2 &&
           "adjacency matrix must be of dimension 2");

    assert(adj_mat.size(0) == adj_mat.size(1) &&
           "adjacency matrix size 0 and size 1 are different");

    auto intOpts = torch::TensorOptions().dtype(torch::kInt64);
    auto floatOpts = torch::TensorOptions().dtype(torch::kFloat32);

    int dimensions = adj_mat.size(0);
    auto pointer =
        torch::where(adj_mat != 0, torch::arange(dimensions).index({None}),
                     torch::full_like(adj_mat, -1, intOpts));

    auto cost_mat =
        torch::where(adj_mat != 0, adj_mat.clone().to(torch::kFloat32),
                     torch::full_like(adj_mat, INFINITY, floatOpts));

    for (int i = 0; i < dimensions; ++i) {
        auto alt_path = cost_mat.index({None, i, Slice()}) +
                        cost_mat.index({Slice(), i, None});

        auto new_cost = torch::minimum(cost_mat, alt_path);

        pointer = torch::where(cost_mat < alt_path, pointer,
                               pointer.index({None, i, Slice()}));
        cost_mat = new_cost;
    }

    return {cost_mat, pointer};
}
