#include <iostream>
#include "algo.hpp"
#include "q_device.hpp"
#include "qft_mapper.hpp"
#include "qft_topo.hpp"
#include "topo.hpp"
#include "util.hpp"

#pragma once

unsigned flow(json& conf, std::vector<unsigned> assign, bool io);

unsigned device_num(json& conf);

unsigned topo_num(json& conf);
