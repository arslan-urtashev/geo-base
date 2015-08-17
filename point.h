#pragma once

#include "typedef.h"

namespace troll {

struct point_t {
	coordinate_t x;
	coordinate_t y;

	point_t(coordinate_t x = 0, coordinate_t y = 0)
		: x(x)
		, y(y)
	{
	}

	point_t operator - (point_t const &p) const
	{
		return point_t(x - p.x, y - p.y);
	}

	bool operator == (point_t const &p) const
	{
		return x == p.x && y == p.y;
	}

	bool operator < (point_t const &p) const
	{
		return x < p.x || (x == p.x && y < p.y);
	}

	int64_t cross(point_t const &p) const
	{
		return 1LL * x * p.y - 1LL * y * p.x;
	}
};


}
