#pragma once

#include "algo.h"

#include <unordered_set>

namespace troll {

template<typename x_t, typename h_t = hash64_t, typename e_t = equal_t>
using unordered_set_t = std::unordered_set<x_t, h_t, e_t>;

}
