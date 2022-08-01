#pragma once
#include <iostream>
#include <vector>
#include "nlohmann/json.hpp"

constexpr size_t ERROR_CODE = (size_t)-1;

using json = nlohmann::json;

template <class T>
std::ostream& operator<<(std::ostream& os, const std::vector<T>& v) {
    os << "[";
    if (v.empty()) {
        return os << "]";
    }
    for (size_t i = 0; i < v.size() - 1; ++i) {
        os << v[i] << ", ";
    }
    return os << v.back() << "]";
}

template <class T>
T json_get(const json& j, const char* key) {
    if (!j.contains(key)) {
        std::cerr << "Necessary key \"" << key << "\" does not exist."
                  << std::endl;
        abort();
    }
    std::cout << "Reading: " << key << "\n";
    return j.at(key).get<T>();
}
