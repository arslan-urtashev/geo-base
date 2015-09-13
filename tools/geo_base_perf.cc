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
#include "location.h"
#include "log.h"
#include "stop_watch.h"

#include <algorithm>
#include <iomanip>
#include <iostream>

using namespace geo_base;

static int ConvertToMicroseconds(double x) {
  return x * 1000000;
}

int main(int argc, char *argv[]) {
  std::cerr << std::fixed << std::setprecision(2);

  LogInit(std::cerr, Log::LEVEL_DEBUG, Log::COLOR_ENABLE);

  if (argc != 2) {
    LogError("geo-base-perf") << "geo-base-perf <geodata.dat>";
    return -1;
  }

  try {
    GeoBase geo_base(argv[1]);
    geo_base.TouchMemory();

    Location location;
    std::vector<Count> checkpoints;

    while (std::cin >> location) {
      StopWatch sw;

      sw.CheckPoint();
      geo_base.Lookup(location);
      double t = sw.CheckPoint();

      checkpoints.push_back(ConvertToMicroseconds(t));
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
