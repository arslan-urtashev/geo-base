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

#ifndef GEO_BASE_LOCATION_H_
#define GEO_BASE_LOCATION_H_

#include "common.h"
#include "io.h"

#include <cmath>

namespace geo_base {

// Location is a point on map with real value coordinates.
// lon - longitude, lat - latitude.
struct Location {
  double lon;
  double lat;

  Location() :
      lon(0),
      lat(0) {
  }

  Location(double lon, double lat) :
      lon(lon),
      lat(lat) {
  }

  Location &operator += (const Location& l) {
    lon += l.lon;
    lat += l.lat;
    return *this;
  }

  double DistTo(const Location& l) const {
    static const double kEarthRadius = 6371000.0;

    double lat1 = ConvertToRadians(lat);
    double lon1 = ConvertToRadians(lon);
    double lat2 = ConvertToRadians(l.lat);
    double lon2 = ConvertToRadians(l.lon);

    return kEarthRadius * acos(sin(lat1) * sin(lat2) +
        cos(lat1) * cos(lat2) * cos(lon1 - lon2));
  }
};

inline InputStream& operator >> (InputStream& in, Location& l) {
  in >> l.lat;
  in.ignore();
  in >> l.lon;
  return in;
}

} // namespace geo_base

#endif // GEO_BASE_LOCATION_H_
