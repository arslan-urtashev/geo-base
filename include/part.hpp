#pragma once

#include "algo.hpp"
#include "edge.hpp"
#include "point.hpp"
#include "common.hpp"

namespace geo_base {

struct part_t {
	coordinate_t coordinate;
	count_t edge_refs_offset;
};

}
