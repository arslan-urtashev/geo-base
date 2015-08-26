#pragma once

#include "algo.h"
#include "edge.h"
#include "point.h"
#include "common.h"

namespace troll {

struct part_t {
	coordinate_t coordinate;
	count_t edge_refs_offset;
};

}
