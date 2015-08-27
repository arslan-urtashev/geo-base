#pragma once

#include "point.h"
#include "common.h"

namespace geo_base {

struct edge_t {
	ref_t beg;
	ref_t end;

	edge_t(ref_t a = 0, ref_t b = 0)
		: beg(a)
		, end(b)
	{
	}

	bool operator == (edge_t const &e) const
	{
		return beg == e.beg && end == e.end;
	}

	bool operator < (edge_t const &e) const
	{
		return beg < e.beg || (beg == e.beg && end < e.end);
	}

	bool lower(edge_t const &e, point_t const *points) const
	{
		if (*this == e)
			return false;

		point_t const &a1 = points[beg];
		point_t const &a2 = points[end];
		point_t const &b1 = points[e.beg];
		point_t const &b2 = points[e.end];

		if (a1 == b1) {
			return (a2 - a1).cross(b2 - a1) > 0;
		} else if (b1.x >= a1.x && b1.x <= a2.x) {
			return (a2 - a1).cross(b1 - a1) > 0;
		} else if (b2.x >= a1.x && b2.x <= a2.x) {
			return (a2 - a1).cross(b2 - a1) > 0;
		} else if (a1.x >= b1.x && a1.x <= b2.x) {
			return (a1 - b1).cross(b2 - b1) > 0;
		} else if (a2.x >= b1.x && a2.x <= b2.x) {
			return (a2 - b1).cross(b2 - b1) > 0;
		} else {
			return false;
		}
	}

	bool contains(point_t const &p, point_t const *points) const
	{
		point_t const &a = points[beg];
		point_t const &b = points[end];
		if (p.x < a.x || p.x > b.x)
			return false;
		return (b - a).cross(p - a) == 0;
	}
};


}
