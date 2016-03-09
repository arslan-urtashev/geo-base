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

#include <geo_base/generator/common.h>
#include <geo_base/library/log.h>

#include <cmath>
#include <cstdlib>
#include <algorithm>

namespace geo_base {
namespace generator {

square_t get_square(dynarray_t<point_t> const &p)
{
    square_t s = 0;
    for (number_t i = 0; i < p.size(); ++i) {
        number_t j = (i + 1 == p.size() ? 0 : i + 1);
        s += p[i].cross(p[j]);
    }
    return s > 0 ? s : -s;
}

static bool is_bad_edge(edge_t const &e, point_t const *p)
{
    return abs(p[e.beg].x - p[e.end].x) > to_coordinate(300.0);
}

dynarray_t<edge_t> make_edges(dynarray_t<point_t> const &points, gen_geo_data_t *geo_data,
    allocator_t *allocator, bool change_direction)
{
    dynarray_t<edge_t> edges(points.size(), allocator);
    for (number_t i = 0; i < points.size(); ++i) {
        number_t j = (i + 1 == points.size() ? 0 : i + 1);

        if (points[i] == points[j])
            continue;

        ref_t const &p1 = geo_data->insert(points[i]);
        ref_t const &p2 = geo_data->insert(points[j]);
        edge_t e(p1, p2);

        point_t const *p = geo_data->points();
        if (p[e.beg].x > p[e.end].x && change_direction)
            std::swap(e.beg, e.end);

        if (is_bad_edge(e, p)) {
            log_warning("Bad edge (%f, %f)=>(%f, %f)",
                to_double(p[e.beg].x), to_double(p[e.beg].y),
                to_double(p[e.end].x), to_double(p[e.end].y));
            continue;
        }

        edges.push_back(e);
    }

    return edges;
}

} // namespace generator
} // namespace geo_base
