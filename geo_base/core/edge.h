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

#pragma once

#include <geo_base/core/common.h>
#include <geo_base/core/point.h>

namespace geo_base {

// edge_t is a type, which represent polygon edge, beg/end refers on begin/end edge points in
// geographical data.
struct edge_t {
    ref_t beg;
    ref_t end;

    edge_t()
        : beg(0)
        , end(0)
    { }

    edge_t(ref_t const &a, ref_t const &b)
        : beg(a)
        , end(b)
    { }

    bool operator == (edge_t const &e) const
    {
        return beg == e.beg && end == e.end;
    }

    bool operator != (edge_t const &e) const
    {
        return beg != e.beg || end != e.end;
    }

    bool operator < (edge_t const &e) const
    {
        return beg < e.beg || (beg == e.beg && end < e.end);
    }

    // Checks that current edge is lying lower then other edge. Both edges must have a common X
    // values, otherwise the behavior is undefined.
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
        } else if (a2 == b2) {
            return (a1 - b1).cross(b2 - b1) > 0;
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

} // namespace geo_base
