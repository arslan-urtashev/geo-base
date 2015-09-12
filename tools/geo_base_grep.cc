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

#include "location.h"
#include "log.h"
#include "util.h"

#include <unistd.h>

#include <iomanip>
#include <iostream>
#include <cstdio>
#include <unordered_set>

using namespace geo_base;

static OutputStream& operator << (OutputStream& out,
    const std::vector<Location>& locations) {
  out << '[';
  for (size_t i = 0; i < locations.size(); ++i) {
    out << '(' << locations[i].lat << ' ' << locations[i].lon << ')';
    out << (i + 1 == locations.size() ? "" : ", ");
  }
  out << ']';
  return out;
}

int main(int argc, char *argv[]) {
  std::cout << std::fixed << std::setprecision(6);

  LogInit(std::cerr, Log::LEVEL_DEBUG, Log::COLOR_ENABLE);
  LogDebug("geo-base-grep") << "geo-base-grep <region_id> [Empty for all polygons]";

  std::unordered_set<RegionID> grep;
  for (int i = 1; i < argc; ++i)
    grep.insert(atoll(argv[i]));

  try {
    std::vector<Location> locations;

    ProtoParser(STDIN_FILENO)([&] (const proto::Region& region) {
      if (grep.empty() || grep.find(region.region_id()) != grep.end()) {
        for (const proto::Polygon& p : region.polygons()) {
          locations.clear();
          for (const proto::Location& l : p.locations())
            locations.push_back(Location(l.lon(), l.lat()));
          PolygonProcessor()(locations, [&] (const std::vector<Location>& l) {
            std::cout << l << std::endl;
          });
        }
      }
    });

  } catch (const std::exception& e) {
    LogError("geo-base-grep", "EXCEPTION") << e.what();
  }

  return 0;
}
