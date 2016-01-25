// Copyright (c) 2016 Urtashev Arslan. All rights reserved.
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
