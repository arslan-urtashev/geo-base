#pragma once

#include "typedef.h"

namespace troll {

inline coordinate_t convert_to_coordinate(double x)
{
	return x * 1e6;
}

inline double convert_to_double(coordinate_t x)
{
	return x / 1e6;
}

}
