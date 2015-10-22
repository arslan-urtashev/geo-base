// Copyright (c) 2015 Urtashev Arslan. All rights reserved.
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

#include <geo_base/polygon.h>

namespace geo_base {

bool polygon_t::contains(point_t const &point, part_t const *parts, ref_t const *edge_refs,
	edge_t const *edges, point_t const *points) const
{
	if (!rectangle.contains(point))
		return false;

	// Find lower bound part, which can contains given point.
	parts += parts_offset;
	part_t const *part = lower_bound(parts, parts_count, point,
		[&] (part_t const &a, point_t const &b)
		{
			return a.coordinate < b.x;
		}
	);

	if (part == parts)
		return false;

	--part;
	if (part + 1 == parts + parts_count)
		return false;

	if (point.x < part->coordinate || point.x > (part + 1)->coordinate)
		return false;

	count_t edge_refs_count = (part + 1)->edge_refs_offset - part->edge_refs_offset;
	return part->contains(point, edge_refs_count, edge_refs, edges, points);
};

bool polygon_t::better(polygon_t const &p, region_t const *regions, count_t regions_count) const
{
	if (square < p.square)
		return true;

	if (square == p.square) {
		region_t const *r1 = find(regions, regions_count, region_id);
		region_t const *r2 = find(regions, regions_count, p.region_id);
		return r1->better(*r2);
	}

	return false;
}

} // namespace geo_base
