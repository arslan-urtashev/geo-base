#pragma once

#include <math.h>
#include <stddef.h>
#include <stdint.h>

namespace troll {

using region_id_t = uint64_t;
using polygon_id_t = int64_t;
using coordinate_t = int32_t;
using ref_t = uint32_t;
using offset_t = uint32_t;
using count_t = uint32_t;
using version_t = uint64_t;
using square_t = int64_t;
using byte_t = uint8_t;
using version_t = uint64_t;

inline coordinate_t constexpr convert_to_coordinate(double x)
{
	return x * 1e6;
}

inline double constexpr convert_to_double(coordinate_t x)
{
	return x / 1e6;
}

inline double constexpr convert_to_radian(double x)
{
	return x * M_PI / 180.0;
}

}
