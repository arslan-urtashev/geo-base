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
#include "get_opt.h"
#include "location.h"
#include "log.h"
#include "stop_watch.h"

#include <algorithm>
#include <iomanip>
#include <iostream>
#include <mutex>
#include <random>
#include <thread>

using namespace geo_base;

static int ConvertToMicroseconds(double x) {
  return x * 1000000;
}

struct Worker {
  std::mt19937 rnd;
  std::vector<Count> checks;
  const GeoBase* geo_base;
  Count iters;
  StopWatch stop_watch;

  explicit Worker(Count init = 0, const GeoBase* geo_base = NULL,
                  Count iters = 0) :
      rnd(init),
      geo_base(geo_base),
      iters(iters) {
  }

  void operator () () {
    for (Count i = 0; i < iters; ++i) {
      Location location;
      location.lon = -180.0 + rnd() * 360.0 / (rnd.max() - rnd.min());
      location.lat = -90.0 + rnd() * 180.0 / (rnd.max() - rnd.min());

      stop_watch.CheckPoint();
      geo_base->Lookup(location);
      double t = stop_watch.CheckPoint();

      checks.push_back(ConvertToMicroseconds(t));
    }
  }
};

int main(int argc, char *argv[]) {
  std::cerr << std::fixed << std::setprecision(2);

  LogInit(std::cerr, Log::LEVEL_DEBUG, Log::COLOR_ENABLE);

  geo_base::Options opts = GetOpts(argc, argv);

  if (argc < 2) {
    LogError("geo-base-perf") << "geo-base-perf <geodata.dat>";
    return -1;
  }

  try {
    GeoBase geo_base(opts.args[0].c_str());
    if (opts.touch_memory)
      LogInfo("geo-base-perf") << "Simple checksum = " << geo_base.TouchMemory();

    std::vector<Count> checkpoints;

    if (opts.random > 0) {
      std::vector<Worker> workers(opts.jobs);
      for (Count i = 0; i < workers.size(); ++i)
        workers[i] = Worker(i, &geo_base, opts.random);

      std::vector<std::thread> threads;
      for (Count i = 0; i < workers.size(); ++i)
        threads.emplace_back(std::ref(workers[i]));

      for (Count i = 0; i < workers.size(); ++i)
        threads[i].join();

      for (const Worker& w : workers)
        checkpoints.insert(checkpoints.end(), w.checks.begin(), w.checks.end());

    } else {
      StopWatch sw;
      Location location;

      while (std::cin >> location) {
        sw.CheckPoint();
        geo_base.Lookup(location);
        double t = sw.CheckPoint();

        checkpoints.push_back(ConvertToMicroseconds(t));
      }
    }

    std::sort(checkpoints.begin(), checkpoints.end());

    for (Count l = 0, r = 0; l < checkpoints.size(); l = r) {
      r = l + 1;
      while (r < checkpoints.size() && checkpoints[r] == checkpoints[l])
        ++r;

      LogInfo("geo-base-perf") << r * 100.0 / checkpoints.size() << "% <= " << checkpoints[l] << "µs";
    }

  } catch (const std::exception& e) {
    LogError("geo-base-perf", "EXCEPTION") << e.what();
  }

  return 0;
}
