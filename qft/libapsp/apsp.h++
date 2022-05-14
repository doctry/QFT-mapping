#pragma once

#include <torch/torch.h>

#include <iostream>
#include <vector>

struct ShortestPath {
    torch::Tensor cost;
    torch::Tensor pointer;
};

std::ostream& operator<<(std::ostream& out, const ShortestPath sp) {
    return out << "Cost:\n"
               << sp.cost << '\n'
               << "Pointer:\n"
               << sp.pointer << "\n\n";
}

ShortestPath apsp(torch::Tensor adj_mat);
