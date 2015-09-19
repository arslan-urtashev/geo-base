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

#include "geo_base.h"
#include "log.h"

#include <iomanip>
#include <iostream>

using namespace geo_base;

struct RegionOutput {
  RegionID region_id;
  const GeoBase& geo_base;

  RegionOutput(RegionID region_id, const GeoBase& geo_base) :
      region_id(region_id),
      geo_base(geo_base) {
  }
};

static OutputStream& operator << (OutputStream& out, const RegionOutput& r) {
  const GeoBase& geo_base = r.geo_base;

  const char* name = geo_base.GetValue(r.region_id, "name:ru");

  if (!name)
    name = geo_base.GetValue(r.region_id, "name");
  if (!name)
    name = geo_base.GetValue(r.region_id, "name:en");

  out << r.region_id << ' ';

  if (name) {
    out << name << ' ';
    const char* en = geo_base.GetValue(r.region_id, "name:en");
    if (en)
      out << "(" << en << ")";
  } else {
    bool sep = false;
    geo_base.ForEachKV(r.region_id,
      [&] (const char* key, const char* val) {
        // if (strstr(key, "name") == key) {
          if (sep)
            out << ", ";
          out << val << " [" << key << "]";
          sep = true;
        // }
      }
    );
  }

  return out;
}

int main(int argc, char *argv[]) {
  LogInit(std::cerr, Log::LEVEL_DEBUG, Log::COLOR_ENABLE);

  if (argc != 2) {
    LogError("geo-base-run")  << "geo-base-run <geodata.dat>";
    return -1;
  }

  try {
    GeoBase geo_base(argv[1]);

    Location location;
    GeoBase::LookupInfo info;

    while (std::cin >> location) {
      RegionID region_id = geo_base.Lookup(location, &info);

      if (region_id != kUnknownRegionID) {
        std::cout << RegionOutput(region_id, geo_base) << '\n';
        for (const RegionID& r : info.regions)
          LogDebug("geo-base-run", region_id) << RegionOutput(r, geo_base);
      } else {
        std::cout << -1 << '\n';
      }
    }

  } catch (const std::exception& e) {
    LogError("geo-base-run", "EXCEPTION") << e.what();
  }

  return 0;
}
