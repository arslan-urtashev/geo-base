#pragma once

#include "library.h"
#include "typedef.h"

namespace troll {

struct box_t {
	static coordinate_t const LOWER_X = convert_to_coordinate(-180.0);
	static coordinate_t const UPPER_X = convert_to_coordinate(180.0);
	static coordinate_t const LOWER_Y = convert_to_coordinate(-90.0);
	static coordinate_t const UPPER_Y = convert_to_coordinate(90.0);
	static coordinate_t const DELTA_X = convert_to_coordinate(1.0);
	static coordinate_t const DELTA_Y = convert_to_coordinate(1.0);
	static coordinate_t const COUNT_X = (UPPER_X - LOWER_X) / DELTA_X;
	static coordinate_t const COUNT_Y = (UPPER_Y - LOWER_Y) / DELTA_Y;

	offset_t polygon_refs_offset;
	count_t polygon_refs_count;
};

}
