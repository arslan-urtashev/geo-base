#pragma once

#include <math.h>
#include <stddef.h>
#include <stdint.h>

namespace troll {

typedef uint64_t region_id_t;
typedef int32_t coordinate_t;
typedef uint32_t ref_t;
typedef uint32_t count_t;
typedef uint64_t version_t;
typedef int64_t square_t;

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

region_id_t const UNKNOWN_REGION_ID = -1;

}
