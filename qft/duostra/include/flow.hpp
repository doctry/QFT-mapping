#include <iostream>
#include "algo.hpp"
#include "q_device.hpp"
#include "qft_mapper.hpp"
#include "qft_topo.hpp"
#include "topo.hpp"
#include "util.hpp"

#pragma once

size_t flow(json& conf, std::vector<size_t> assign, bool io);

size_t device_num(json& conf);

size_t topo_num(json& conf);
