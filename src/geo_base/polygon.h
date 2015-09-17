// Copyright (c) 2015 Urtashev Arslan. All rights reserved.
// Contacts: <urtashev@gmail.com>
//   
// Permission is hereby granted, free of charge, to any person obtaining
// a copy of this software and associated documentation files (the
// "Software"), to deal in the Software without restriction, including
// without limitation the rights to use, copy, modify, merge, publish,
// distribute, sublicense, and/or sell copies of the Software, and to
// permit persons to whom the Software is furnished to do so, subject to
// the following conditions:
//           
//   The above copyright notice and this permission notice shall be included
//   in all copies or substantial portions of the Software.
//              
// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,
// EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF
// MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.
// IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY
// CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT,
// TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE
// SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.

#ifndef GEO_BASE_POLYGON_H_
#define GEO_BASE_POLYGON_H_

#include "algo.h"
#include "common.h"
#include "edge.h"
#include "part.h"
#include "point.h"
#include "rectangle.h"
#include "region.h"

namespace geo_base {

// Polygon is a representation of persistent scanline data structure.
struct Polygon {
  // If true and polygon contains given point, this means that region with
  // region_id does not contains point.
  bool is_inner;

  // Geographical data indetifiers.
  RegionID region_id;
  PolygonID polygon_id;

  // Versions of persistent scanline.
  Count parts_count;
  Count parts_offset;

  // Rectangle in which lies that polygon.
  Rectangle rectangle;

  // Square of polygon. Need for determine which polygon is better.
  // See Better member function.
  Square square;

  // Fast point in polygon test using persistent scanline. You can see how
  // this data structure generated in GeoBaseGenerate.
  bool Contains(const Point& point, const Part* parts, const Ref* edge_refs,
      const Edge* edges, const Point* points) const;

  // Check that current polygon better then given polygon, which means that
  // current polygons lying deeper then given in polygons hierarchy.
  bool Better(const Polygon& p, const Region* regions, Count regions_count) const;
};

} // namespace geo_base

#endif // GEO_BASE_POLYGON_H_
