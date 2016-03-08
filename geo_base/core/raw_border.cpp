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

#include <geo_base/core/raw_border.h>

namespace geo_base {

bool raw_border_t::contains(point_t const &point, ref_t const *edge_refs, edge_t const *edges,
    point_t const *points) const
{
    if (!rectangle.contains(point))
        return false;

    edge_refs += edge_refs_offset;
    
    number_t intersections = 0;
    for (number_t i = 0; i < edge_refs_number; ++i) {
        edge_t const &e = edges[edge_refs[i]];

        if (e.contains(point, points))
            return true;

        point_t const &a = points[e.beg];
        point_t const &b = points[e.end];

        if (a.x < point.x && b.x >= point.x && e.lower(point, points))
            ++intersections;
    }

    return intersections % 2 == 1;
}

bool raw_border_t::better(raw_border_t const &b, region_t const *regions,
    number_t regions_number) const
{
    if (square < b.square)
        return true;

    if (square == b.square) {
        region_t const *begin = regions;
        region_t const *end = regions + regions_number;

        region_t const *r1 = std::lower_bound(begin, end, region_id);
        region_t const *r2 = std::lower_bound(begin, end, b.region_id);

        if (r1 == end || r1->region_id != region_id)
            return false;

        if (r2 == end || r2->region_id != b.region_id)
            return false;

        return r1->better(*r2);
    }

    return false;
}

} // namespace geo_base
