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

#include "part.h"

namespace geo_base {

bool Part::Contains(const Point& point, const Count edge_refs_count,
    const Ref* edge_refs, const Edge* edges, const Point* points) const {
  edge_refs += edge_refs_offset;

  // Find lower bound edge, which lying below given point.
  const Ref* edge_ref = LowerBound(edge_refs, edge_refs_count, point,
    [&] (const Ref& e, const Point& p) {
      if (edges[e].Contains(p, points))
        return false;
      const Point& a = points[edges[e].beg];
      const Point& b = points[edges[e].end];
      return (b - a).CrossProduct(p - a) > 0;
    }
  );

  if (edge_ref == edge_refs + edge_refs_count)
    return false;

  if (edges[*edge_ref].Contains(point, points))
    return true;

  // If the point is on the inside of the polygon then it will intersect the
  // edge an odd number of times.
  return (edge_ref - edge_refs) % 2 == 1;
}

} // geo_base
