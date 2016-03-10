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
#include <geo_base/core/edge.h>
#include <geo_base/core/part.h>
#include <geo_base/core/point.h>
#include <geo_base/core/bbox.h>
#include <geo_base/core/region.h>

namespace geo_base {

struct polygon_base_t {
    enum type_t {
        TYPE_UNKNOWN = 0,
        TYPE_INNER   = 1,
        TYPE_OUTER   = 2,
    };

    // If TYPE_INNER and polygon contains given point, this means that region with region_id
    // does not contains point.
    type_t type;

    // Geographical data indetifiers.
    geo_id_t region_id;
    geo_id_t polygon_id;

    // Rectangle in which lies that polygon.
    bbox_t bbox;

    // Square of polygon. Need for determine which polygon is better. See better member function.
    square_t square;

    // Total points number of given polygon.
    number_t points_number;

    // Check that this polygon better then given polygon, which means that this polygons lying
    // deeper then given in polygons hierarchy.
    bool better(polygon_base_t const &p, region_t const *regions, number_t regions_number) const;
};

// Polygon is a representation of persistent scanline data structure.
struct polygon_t : public polygon_base_t {
    // Versions of persistent scanline.
    number_t parts_offset;
    number_t parts_number;

    // Fast point in polygon test using persistent scanline. You can see how this data structure
    // generated in geo_base/generator/.
    bool contains(point_t const &point, part_t const *parts, ref_t const *edge_refs,
        edge_t const *edges, point_t const *points) const;
};

// Raw polygon is a polygon representation for slow tests.
struct raw_polygon_t : public polygon_base_t {
    // Raw polygon edge refs.
    number_t edge_refs_offset;
    number_t edge_refs_number;

    bool contains(point_t const &point, ref_t const *edge_refs, edge_t const *edges,
        point_t const *points) const;
};

} // namespace geo_base
