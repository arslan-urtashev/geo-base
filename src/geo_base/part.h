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

#ifndef GEO_BASE_PART_H_
#define GEO_BASE_PART_H_

#include "algo.h"
#include "common.h"
#include "edge.h"
#include "point.h"

namespace geo_base {

// Part contains version of persistent scanline. Parts lying in GeoData.parts
// array, ordered by coordinate for each polygon. Variable edge_refs_offset 
// refers on edge_refs array for current part. For optimal usage of memory,
// Part does not contain edge_refs_count variable, because it's can be computed
// as parts[i + 1].edge_refs_offset - parts[i].edge_refs_offset for every part
// in GeoData. Especially for this added fake part into GeoData with right
// edge_refs_offset. Refs in edge_refs are in ascending order for each part.
// It is necessary to quickly determine how many edges is under the point.
// See GeoBaseGenerate for details.
struct Part {
  Coordinate coordinate;
  Count edge_refs_offset;

  // Checks point lying under odd numbers of edges or on edge.
  bool Contains(const Point& point, const Count edge_refs_count,
      const Ref* edge_refs, const Edge* edges, const Point* points) const;
};

} // namespace geo_base

#endif // GEO_BASE_PART_H_
