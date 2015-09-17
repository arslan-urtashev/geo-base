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
#include "get_opt.h"
#include "log.h"

#include <atomic>
#include <cassert>
#include <iomanip>
#include <iostream>
#include <random>
#include <thread>
#include <unordered_map>
#include <unordered_set>

using namespace geo_base;

static const double kLookupRadius = 0.001;
static const uint32_t kLookupCount = 10;

struct Worker {
  const GeoDataWrapper* wrapper;
  std::mt19937 generator;
  std::unordered_set<PolygonID> polygons;
  std::unordered_map<RegionID, RegionID> parents;
  Count points_offset;
  Count points_count;
  std::atomic<Count> offset;

  double GetRand() {
    return -1.0 + generator() * 2.0 / (generator.max() - generator.min());
  }

  explicit Worker(const GeoDataWrapper* g = NULL, Count offset = 0,
                  Count count = 0, Count seed = 0) :
      wrapper(g),
      generator(seed),
      points_offset(offset),
      points_count(count),
      offset(offset) {
  }

  Worker& operator = (const Worker& w) {
    wrapper = w.wrapper;
    polygons = w.polygons;
    generator = w.generator;
    points_offset = w.points_offset;
    points_count = w.points_count;
    offset = points_offset;
    return *this;
  }

  void operator () () {
    LookupInfo info;
    for (offset = points_offset; offset < points_offset + points_count; ++offset) {
      for (Count try_lookup = 0; try_lookup < kLookupCount; ++try_lookup) {
        const Point& p = wrapper->geo_data()->points[offset];

        double lon = ConvertToDouble(p.x) + GetRand() * kLookupRadius;
        double lat = ConvertToDouble(p.y) + GetRand() * kLookupRadius;

        RegionID region_id = GeoDataLookup(*wrapper->geo_data(),
            Location(lon, lat), &info);

        if (region_id != kUnknownRegionID)
          polygons.insert(info.polygon_id);

        for (size_t i = 0; i + 1 < info.regions.size(); ++i)
          parents[info.regions[i]] = info.regions[i + 1];
      }
    }
  }
};

static OutputStream& operator << (OutputStream& out,
    const std::vector<Worker>& workers) {
  Count count = 0, total = 0;
  for (const Worker& w : workers) {
    count += (w.offset - w.points_offset);
    total += w.points_count;
  }
  Count result = count * 20.0 / total;
  out << "[";
  for (Count i = 1; i <= 20; ++i)
    out << (i <= result ? "#" : " ");
  out << "] ";
  out << count * 100.0 / total;
  return out;
}

int main(int argc, char *argv[]) {
  std::cerr << std::fixed << std::setprecision(2);

  LogInit(std::cerr, Log::LEVEL_DEBUG, Log::COLOR_ENABLE);

  if (argc < 2) {
    LogError("geo-base-sift") << "geo-base-sift <geodata.dat>";
    return -1;
  }

  std::vector<std::string> args;
  geo_base::Options opts = geo_base::GetOpts(argc, argv, &args);

  try {
    GeoDataWrapper wrapper(args[0].c_str());

    Count points_count = wrapper.geo_data()->points_count;
    Count one_thread_count = points_count / opts.jobs + 1;

    std::vector<Worker> workers(opts.jobs);
    for (Count i = 0, offset = 0; i < opts.jobs && offset < points_count; ++i, offset += one_thread_count)
      workers[i] = Worker(&wrapper, offset, std::min(one_thread_count, points_count - offset), i);

    std::vector<std::thread> threads(opts.jobs);
    for (size_t i = 0; i < threads.size(); ++i)
      threads[i] = std::thread(std::ref(workers[i]));

    while (true) {
      bool completed = true;

      for (size_t i = 0; i < workers.size(); ++i)
        if (workers[i].offset != workers[i].points_offset + workers[i].points_count)
          completed = false;

      LogStatus("geo-base-sift", "monte-carlo") << workers;

      if (completed)
        break;

      std::this_thread::sleep_for(std::chrono::seconds(1));
    }

    for (Count i = 0; i < threads.size(); ++i)
      threads[i].join();
    LogStatus::Clear();

    std::unordered_set<PolygonID> polygons;
    std::unordered_map<RegionID, RegionID> parents;

    for (Count i = 0; i < workers.size(); ++i) {
      polygons.insert(workers[i].polygons.begin(), workers[i].polygons.end());
      workers[i].polygons.clear();

      parents.insert(workers[i].parents.begin(), workers[i].parents.end());
      workers[i].parents.clear();
    }

    Count count = 0;
    Count filt_count = 0;

    ProtoWriter writer(STDOUT_FILENO);

    proto::Region buf_region;
    std::string buf;

    ProtoParser(STDIN_FILENO)([&] (const proto::Region& region) {
      buf_region = region;

      buf_region.mutable_polygons()->Clear();
      for (const proto::Polygon& p : region.polygons()) {
        if (polygons.find(p.polygon_id()) != polygons.end())
          *buf_region.add_polygons() = p;
        else
          ++filt_count;
        ++count;
      }

      if (parents.find(buf_region.region_id()) != parents.end())
        buf_region.set_parent_id(parents[buf_region.region_id()]);

      buf.clear();
      buf_region.SerializeToString(&buf);
      writer.Write(buf);
    });

    LogInfo("geo-base-sift") << "All polygons count = " << count;
    LogInfo("geo-base-sift") << "Sift polygons count = " << filt_count;

  } catch (const std::exception& e) {
    LogError("geo-base-sift", "EXCEPTION") << e.what();
  }
  
  return 0;
}
