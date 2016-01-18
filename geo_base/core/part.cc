// copyright (c) 2015, 2016 urtashev arslan. all rights reserved.
// contacts: <urtashev@gmail.com>
//
// permission is hereby granted, free of charge, to any person obtaining a copy of this software
// and associated documentation files (the "software"), to deal in the software without
// restriction, including without limitation the rights to use, copy, modify, merge, publish,
// distribute, sublicense, and/or sell copies of the software, and to permit persons to whom the
// software is furnished to do so, subject to the following conditions:
//
//   the above copyright notice and this permission notice shall be included in all copies or
//   substantial portions of the software.
//
// the software is provided "as is", without warranty of any kind, express or implied, including
// but not limited to the warranties of merchantability, fitness for a particular purpose and
// noninfringement. in no event shall the authors or copyright holders be liable for any claim,
// damages or other liability, whether in an action of contract, tort or otherwise, arising from,
// out of or in connection with the software or the use or other dealings in the software.

#include <algorithm>
#include <geo_base/core/part.h>

namespace geo_base {

bool part_t::contains(point_t const &point, number_t edge_refs_number, ref_t const *edge_refs,
	edge_t const *edges, point_t const *points) const
{
	edge_refs += edge_refs_offset;
	ref_t const *edge_refs_end = edge_refs + edge_refs_number;

	// Find lower bound edge, which lying below given point.
	ref_t const *edge_ref = std::lower_bound(edge_refs, edge_refs_end, point,
		[&] (ref_t const &e, point_t const &p)
		{
			if (edges[e].contains(p, points))
				return false;
			point_t const &a = points[edges[e].beg];
			point_t const &b = points[edges[e].end];
			return (b - a).cross(p - a) > 0;
		}
	);

	if (edge_ref == edge_refs + edge_refs_number)
		return false;

	if (edges[*edge_ref].contains(point, points))
		return true;

	// If the point is inside of the polygon then it will intersect the edge an odd number of times.
	return (edge_ref - edge_refs) % 2 == 1;
}

} // namespace geo_base
