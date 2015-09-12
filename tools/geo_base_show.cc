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

#include "geo_data.h"
#include "geo_data_wrapper.h"
#include "hash.h"
#include "log.h"

#include <iomanip>
#include <iostream>
#include <map>
#include <unordered_map>
#include <unordered_set>
#include <vector>

using namespace geo_base;

template<typename Callback>
static void ForEachPolygon(const GeoData* dat, Callback callback) {
  for (Count i = 0; i < dat->polygons_count; ++i)
    callback(&(dat->polygons[i]));
}

template<typename Callback>
static void ForEachPart(const GeoData* dat, const Polygon* p,
    Callback callback) {
  for (Count i = p->parts_offset; i < p->parts_offset + p->parts_count; ++i)
    callback(&(dat->parts[i]));
}

template<typename Callback>
static void ForEachRef(const GeoData* dat, const Part* part,
    Callback callback) {
  for (Count i = part->edge_refs_offset; i < (part + 1)->edge_refs_offset; ++i)
    callback(dat->edge_refs[i]);
}

template<typename callback>
static void ForEach(const GeoData* dat) {
  ForEachPolygon(dat, [&] (const Polygon* polygon) {
    ForEachPart(dat, polygon, [&] (const Part* part) {
      ForEachRef(dat, part, [&] (const Ref& ref) {
        callback(polygon, part, ref);
      });
    });
  });
}

template<typename Vector, typename Callback>
static void ForEachUniq(const Vector& a, Callback callback) {
  for (Count l = 0, r = 0; l < a.size(); l = r) {
    r = l;
    while (r < a.size() && a[l] == a[r])
      ++r;
    callback(l, r);
  }
}

int main(int argc, char *argv[]) {
  std::cout << std::fixed << std::setprecision(2);

  LogInit(std::cerr, Log::LEVEL_DEBUG, Log::COLOR_ENABLE);

  if (argc != 2) {
    LogError("geo-base-show") << "geo-base-show <geodata.dat>";
    return -1;
  }

  try {
    GeoDataWrapper wrapper(argv[1]);
    const GeoData* dat = wrapper.geo_data();

    GeoDataShow(*dat, std::cerr);

    {
      std::vector<Count> part_refs;
      ForEachPolygon(dat, [&] (const Polygon* polygon) {
        ForEachPart(dat, polygon, [&] (const Part* part) {
          Count count = (part + 1)->edge_refs_offset - part->edge_refs_offset;
          part_refs.push_back(count);
        });
      });

      std::sort(part_refs.begin(), part_refs.end());
      Count part_refs_memory = 0;

      ForEachUniq(part_refs, [&] (Count l, Count r) {
        for (Count i = l; i < r; ++i)
          part_refs_memory += part_refs[i] * sizeof(i);

        double percent = r * 100.0 / part_refs.size();
        double memory = part_refs_memory / (1024. * 1024.0);

        LogInfo("geo-base-show", "refs count stat") << percent << "% <= "
            << part_refs[l] << " (" << memory << " MB)";
      });
    }

  } catch (const std::exception& e) {
    LogError("geo-base-show", "EXCEPTION") << e.what();
  }

  return 0;
}
