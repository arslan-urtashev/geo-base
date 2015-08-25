#pragma once

#include "algo.h"
#include "hash.h"

#include <unordered_map>

namespace troll {

template<typename x_t, typename y_t, typename h_t = hash64_t, typename e_t = equal_t>
using unordered_map_t = std::unordered_map<x_t, y_t, h_t, e_t>;

}
