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

#include <geo_base/core/geo_base.h>
#include <geo_base/core/geo_data/map.h>
#include <geo_base/core/location.h>
#include <geo_base/core/geo_data/debug.h>
#include <geo_base/library/log.h>
#include <geo_base/library/mem_file.h>

using namespace geo_base;

static char const *get_output(geo_id_t const region_id, geo_base_t const &geo_base)
{
    char const *output = nullptr;
    
    geo_base.each_kv(region_id, [&] (char const *k, char const *v) {
        if (!strcmp(k, "name:en"))
            output = v;
    });

    if (output)
        return output;

    geo_base.each_kv(region_id, [&] (char const *k, char const *v) {
        if (!strcmp(k, "name"))
            output = v;
    });

    return output;
}

int main(int argc, char *argv[])
{
    std::ios_base::sync_with_stdio(false);
    log_setup(STDERR_FILENO, LOG_LEVEL_DEBUG);

    if (argc < 2) {
        log_error("USAGE: geo-base-run <geo-base.dat> [debug:0/1]");
        return -1;
    }

    std::vector<geo_id_t> regions;
    bool const debug = argc == 2 ? false : atoi(argv[2]);

    geo_base_t geo_base(argv[1]);

    if (debug)
        geo_data::show(log_fd(), geo_base.geo_data());

    while (true) {
        location_t location;
        if (!(std::cin >> location.lat))
            break;
        std::cin.ignore();
        if (!(std::cin >> location.lon))
            break;

        geo_id_t region_id = geo_base.lookup(location, debug ? &regions : nullptr);
        if (region_id == UNKNOWN_GEO_ID) {
            std::cout << "unknown" << std::endl;
            continue;
        }

        std::cout << region_id << std::endl;

        if (!debug) {
            geo_base.each_kv(region_id, [&] (char const *k, char const *v) {
                log_debug("k=\"%s\" v=\"%s\"", k, v);
            });
        } else {
            for (geo_id_t const &region_id : regions)
                log_debug("%s (%lu)", get_output(region_id, geo_base), region_id);
        }
    }

    return 0;
}
