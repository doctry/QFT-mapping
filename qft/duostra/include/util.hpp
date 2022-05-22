#pragma once

#include <iostream>
#include <vector>

template <class T>
std::ostream &operator<<(std::ostream &os, const std::vector<T> &v) {
    os << "[";
    if (v.empty()) {
        return os << "]";
    }
    for (unsigned i = 0; i < v.size() - 1; ++i) {
        os << v[i] << ", ";
    }
    return os << v.back() << "]";
}