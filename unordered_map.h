#pragma once

#include "algo.h"

#include <unordered_map>

namespace troll {

template<typename x_t, typename y_t, typename h_t, typename e_t>
using unordered_map_t = std::unordered_map<x_t, y_t, h_t, e_t>;

}
