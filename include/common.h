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

#ifndef GEO_BASE_COMMON_H_
#define GEO_BASE_COMMON_H_

#include <cmath>
#include <cstddef>
#include <cstdint>

namespace geo_base {

// Coordinate is a type, which represent real value coordinate in integer
// type. Equals ConvertToCoordinate(x), where x is a real value.
typedef int32_t Coordinate;

// Square is a type, which represent square of a polygon in integer type.
typedef int64_t Square;

// Count is a type, needed for optimal storage size values in GeoData.
typedef uint32_t Count;

// Ref just index type.
typedef uint32_t Ref;

// Identifiers of geo objects, stored in GeoData.
typedef uint64_t RegionID;
typedef uint64_t PolygonID;

// Version is a type of GeoData version. Logic of filling GeoData version
// is in geo_data.cc file
typedef uint64_t Version;

inline constexpr Coordinate ConvertToCoordinate(double x) {
  return x * 1e6;
}

inline constexpr double ConvertToDouble(Coordinate x) {
  return x / 1e6;
}

inline constexpr double ConvertToRadians(double degrees) {
  return degrees * M_PI / 180.0;
}

const RegionID kUnknownRegionID = -1;

} // namespace geo_base

#endif // GEO_BASE_COMMON_H_
