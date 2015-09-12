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

#ifndef GEO_BASE_RECTANGLE_H_
#define GEO_BASE_RECTANGLE_H_

#include "common.h"

namespace geo_base {

struct Rectangle {
  Coordinate x1;
  Coordinate y1;
  Coordinate x2;
  Coordinate y2;

  Rectangle() :
      x1(0), y1(0), x2(0), y2(0) {
  }

  Rectangle(Coordinate x1, Coordinate y1, Coordinate x2, Coordinate y2) :
      x1(x1), y1(y1), x2(x2), y2(y2) {
  }

  void Init() {
    x1 = ConvertToCoordinate(180.0);
    y1 = ConvertToCoordinate(90.0);
    x2 = ConvertToCoordinate(-180.0);
    y2 = ConvertToCoordinate(-90.0);
  }

  void Relax(const Point& p) {
    x1 = Min(x1, p.x);
    y1 = Min(y1, p.y);
    x2 = Max(x2, p.x);
    y2 = Max(y2, p.y);
  }

  bool HasIntersections(const Rectangle& r) const {
    if (x1 > r.x2 || x2 < r.x1 || y1 > r.y2 || y2 < r.y1)
      return false;
    return true;
  }

  bool Contains(const Point& p) const {
    if (p.x < x1 || p.x > x2 || p.y < y1 || p.y > y2)
      return false;
    return true;
  }
};

} // namespace geo_base

#endif
