// Copyright (c) 2015,2016 Urtashev Arslan. All rights reserved.
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

#include <geo_base/edge.h>
#include <geo_base/part.h>
#include <geo_base/point.h>
#include <geo_base/rectangle.h>
#include <geo_base/region.h>
#include <geo_base/typedef.h>

namespace geo_base {

// Polygon is a representation of persistent scanline data structure.
struct polygon_t {
	// If true and polygon contains given point, this means that region with region_id does not
	// contains point.
	bool is_inner;

	// Geographical data indetifiers.
	geo_id_t region_id;
	geo_id_t polygon_id;

	// Versions of persistent scanline.
	count_t parts_offset;
	count_t parts_count;

	// Rectangle in which lies that polygon.
	rectangle_t rectangle;

	// Square of polygon. Need for determine which polygon is better. See better member function.
	square_t square;

	// Fast point in polygon test using persistent scanline. You can see how this data structure
	// generated in geo_base/generator/.
	bool contains(point_t const &point, part_t const *parts, ref_t const *edge_refs,
		edge_t const *edges, point_t const *points) const;

	// Check that this polygon better then given polygon, which means that this polygons lying
	// deeper then given in polygons hierarchy.
	bool better(polygon_t const &p, region_t const *regions, count_t regions_count) const;
};

} // namespace geo_base
