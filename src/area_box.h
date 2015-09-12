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

#ifndef GEO_BASE_AREA_BOX_H_
#define GEO_BASE_AREA_BOX_H_

#include "common.h"
#include "util.h"

namespace geo_base {

// Area of geo territory. Variable polygon_refs_offset refers to the
// polygons lying inside this area. Geo map is divided into equal rectangles 
// from (kLowerX, kLowerY) to (kUpperX, kUpperY) with kDeltaX and 
// kDeltaY sizes. Logic of filling is in GeoBaseGenerate.GenerateBoxes
struct AreaBox {
  static const Coordinate kLowerX = ConvertToCoordinate(-180.0);
  static const Coordinate kUpperX = ConvertToCoordinate(180.0);
  static const Coordinate kLowerY = ConvertToCoordinate(-90.0);
  static const Coordinate kUpperY = ConvertToCoordinate(90.0);
  static const Coordinate kDeltaX = ConvertToCoordinate(1.0);
  static const Coordinate kDeltaY = ConvertToCoordinate(1.0);
  static const Coordinate kCountX = (kUpperX - kLowerX) / kDeltaX;
  static const Coordinate kCountY = (kUpperY - kLowerY) / kDeltaY;

  Count polygon_refs_offset;
  Count polygon_refs_count;
};

} // namespace geo_base

#endif // GEO_BASE_AREA_BOX_H_
