// Copyright (c) 2015, 2016 Urtashev Arslan. All rights reserved.
// Contacts: <urtashev@gmail.com>
//   
// Permission is hereby granted, free of charge, to any person obtaining a copy of this software
// and associated documentation files (the "Software"), to deal in the Software without
// restriction, including without limitation the rights to use, copy, modify, merge, publish,
// distribute, sublicense, and/or sell copies of the Software, and to permit persons to whom the
// Software is furnished to do so, subject to the following conditions:
//
//   The above copyright notice and this permission notice shall be included in all copies or
//   substantial portions of the Software.
//
// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, INCLUDING
// BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND
// NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM,
// DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
// OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.

#pragma once

#include <geo_base/core/common.h>
#include <geo_base/core/point.h>

#include <algorithm>

namespace geo_base {

struct rectangle_t {
	coordinate_t x1;
	coordinate_t y1;
	coordinate_t x2;
	coordinate_t y2;

	rectangle_t()
		: x1(0)
		, y1(0)
		, x2(0)
		, y2(0)
	{
	}

	rectangle_t(coordinate_t x1, coordinate_t y1, coordinate_t x2, coordinate_t y2)
		: x1(x1)
		, y1(y1)
		, x2(x2)
		, y2(y2)
	{
	}

	rectangle_t(point_t const *points, number_t number)
	{
		init();
		for (number_t i = 0; i < number; ++i)
			relax(points[i]);
	}

	void init()
	{
		x1 = to_coordinate(180.0);
		y1 = to_coordinate(90.0);
		x2 = to_coordinate(-180.0);
		y2 = to_coordinate(-90.0);
	}

	void relax(point_t const &p)
	{
		x1 = std::min(x1, p.x);
		y1 = std::min(y1, p.y);
		x2 = std::max(x2, p.x);
		y2 = std::max(y2, p.y);
	}

	bool has_intersection(rectangle_t const &r) const
	{
		if (x1 > r.x2 || x2 < r.x1 || y1 > r.y2 || y2 < r.y1)
			return false;
		return true;
	}

	bool contains(point_t const &p) const
	{
		if (p.x < x1 || p.x > x2 || p.y < y1 || p.y > y2)
			return false;
		return true;
	}
};

} // namespace geo_base
