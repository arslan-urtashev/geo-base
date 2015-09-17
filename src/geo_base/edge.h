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

#ifndef GEO_BASE_EDGE_H_
#define GEO_BASE_EDGE_H_

#include "common.h"
#include "point.h"

namespace geo_base {

// Edge is a type, which represent polygon edge, beg/end refers on begin/end
// edge points in GeoData points array.
struct Edge {
  Ref beg;
  Ref end;

  Edge() :
      beg(0),
      end(0) {
  }

  Edge(Ref a, Ref b) :
      beg(a),
      end(b) {
  }

  bool operator == (const Edge& e) const {
    return beg == e.beg && end == e.end;
  }

  bool operator < (Edge const &e) const {
    return beg < e.beg || (beg == e.beg && end < e.end);
  }

  // Checks that current edge is lying lower then other edge.
  // Both edges must have a common X values, otherwise the behavior 
  // is undefined.
  bool Lower(const Edge& e, const Point *points) const;

  // Checks that current edge contains given point.
  bool Contains(const Point& p, const Point* points) const;
};

} // namespace geo_base

#endif // GEO_BASE_EDGE_H_
