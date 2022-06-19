#pragma once
#include "nlohmann/json.hpp"
#include <iostream>
#include <vector>

using json = nlohmann::json;

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

template <class T> T json_get(json &j, const char *key) {
    if (!j.contains(key)) {
        std::cerr << "Necessary key \"" << key << "\" does not exist."
                  << std::endl;
        abort();
    }
    return j[key].get<T>();
}