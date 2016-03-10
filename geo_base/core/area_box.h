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

namespace area_box {

coordinate_t const lower_x = to_coordinate(-180.0);
coordinate_t const upper_x = to_coordinate(180.0);
coordinate_t const lower_y = to_coordinate(-90.0);
coordinate_t const upper_y = to_coordinate(90.0);
coordinate_t const delta_x = to_coordinate(0.1);
coordinate_t const delta_y = to_coordinate(0.1);
coordinate_t const number_x = (upper_x - lower_x) / delta_x;
coordinate_t const number_y = (upper_y - lower_y) / delta_y;
coordinate_t const number = number_x * number_y;

} // namespace area_box

// Area of geo territory. Variable polygon_refs_offset refers to the polygons lying inside this
// area. Geo map is divided into equal bboxes from (area_box::lower_x, area_box::lower_y) to
// (area_box::upper_x, area_box::upper_y) with delta_x and delta_y sizes. Logic of filling is in
// generator.
struct area_box_t {
    number_t polygon_refs_offset;
    number_t polygon_refs_number;
};

// Determine in wich area box in geo_data is point.
ref_t lookup_area_box(point_t const &point);

} // namespace geo_base
