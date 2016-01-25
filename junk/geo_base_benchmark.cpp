// Copyright (c) 2016 Urtashev Arslan. All rights reserved.
// Contacts: <urtashev@gmail.com>
//
// Permission is hereby granted, free of charge, to any person obtaining a copy of this software
// and associated documentation files (the "Software"), to deal in the Software without
// restriction, including without limitation the rights to use, copy, modify, merge, publish,
// distribute, sublicense, and/or sell copies of the Software, and to permit persons to whom the
// Software is furnished to do so, subject to the following conditions:
//
//   The above copyright notice and this permission notice shall be included in all copies or
//   substantial portions of the Software.
//
// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, INCLUDING
// BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND
// NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM,
// DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
// OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.

#include <iostream>
#include <thread>
#include <vector>
#include <random>

#include <geo_base/core/geo_base.h>
#include <geo_base/core/geo_data/debug.h>
#include <geo_base/core/geo_data/map.h>
#include <geo_base/core/location.h>
#include <geo_base/lib/log.h>
#include <geo_base/lib/mem_file.h>
#include <geo_base/lib/stop_watch.h>

using namespace geo_base;

static size_t const LOOKUPS_NUMBER = 1e6;
static size_t const THREADS_NUMBER = 4;

static location_t random_location(std::mt19937 &rnd)
{
    location_t l;
    l.lat = rnd() * 180.0 / RAND_MAX - 90.0;
    l.lon = rnd() * 360.0 / RAND_MAX - 180.0;
    return l;
}

int main(int argc, char *argv[])
{
    log_setup(STDERR_FILENO, LOG_LEVEL_DEBUG);

    if (argc != 2) {
        log_error("USAGE: geo-base-benchmark <geo-base.dat>");
        return -1;
    }

    geo_base_t geo_base(argv[1]);

    std::vector<std::thread> threads(THREADS_NUMBER);
    std::vector<std::vector<float>> seconds(THREADS_NUMBER, std::vector<float>(LOOKUPS_NUMBER));

    for (size_t i = 0; i < THREADS_NUMBER; ++i) {
        threads[i] = std::thread([&geo_base, &seconds, i] () {
            log_info("[%lu] Run benchmark", i + 1);

            std::mt19937 random(i + 337);

            std::vector<location_t> locations(LOOKUPS_NUMBER);
            for (size_t j = 0; j < locations.size(); ++j)
                locations[j] = random_location(random);
        
            for (size_t j = 0; j < locations.size(); ++j) {
                stop_watch_t stop_watch;
                stop_watch.run();

                geo_base.lookup(locations[j]);

                seconds[i][j] = stop_watch.get();
            }
        });
    }

    for (size_t i = 0; i < THREADS_NUMBER; ++i)
        threads[i].join();

    std::vector<float> total;
    for (size_t i = 0; i < THREADS_NUMBER; ++i)
        total.insert(total.end(), seconds[i].begin(), seconds[i].end());

    std::sort(total.begin(), total.end());

    for (int i = 1; i <= 99; ++i)
        log_info("[%2d%%] <= %.6f", i, total[std::min(total.size() - 1, i * total.size() / 100)]);

    for (int i = 1; i <= 100; ++i)
        log_info("[%2d%.2d] <= %6f", (i == 100 ? 100 : 99), (i == 100 ? 0 : i), 
            total[std::min(total.size() - 1, (99 * 100 + i) * total.size() / 10000)]);

    return 0;
}
