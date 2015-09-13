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

#ifndef GEO_BASE_POINT_H_
#define GEO_BASE_POINT_H_

#include "common.h"
#include "location.h"

namespace geo_base {

// Point is a representation of Location with integer coordinates.
struct Point {
  Coordinate x;
  Coordinate y;

  Point() :
      x(0),
      y(0) {
  }

  Point(Coordinate x, Coordinate y) :
      x(x),
      y(y) {
  }

  explicit Point(const Location& l) :
      x(ConvertToCoordinate(l.lon)),
      y(ConvertToCoordinate(l.lat)) {
  }

  Point operator - (const Point& p) const {
    return Point(x - p.x, y - p.y);
  }

  bool operator == (const Point& p) const {
    return x == p.x && y == p.y;
  }

  bool operator != (const Point& p) const {
    return !(*this == p);
  }

  bool operator < (const Point& p) const {
    return x < p.x || (x == p.x && y < p.y);
  }

  Square CrossProduct(const Point& p) const {
    return 1LL * x * p.y - 1LL * y * p.x;
  }
};

inline OutputStream& operator << (OutputStream& out, const Point& p) {
  out << "(" << p.x << ", " << p.y << ")";
  return out;
}

} // namespace geo_base

#endif // GEO_BASE_POINT_H_
