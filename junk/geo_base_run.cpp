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
#include <geo_base/lib/log.h>
#include <geo_base/lib/mem_file.h>

using namespace geo_base;

int main(int argc, char *argv[])
{
    std::ios_base::sync_with_stdio(false);
    log_setup(STDERR_FILENO, LOG_LEVEL_DEBUG);

    if (argc != 2) {
        log_error("USAGE: geo-base-run <geo-base.dat>");
        return -1;
    }

    geo_base_t geo_base(argv[1]);

    while (true) {
        location_t location;
        if (!(std::cin >> location.lat))
            break;
        std::cin.ignore();
        if (!(std::cin >> location.lon))
            break;

        geo_id_t region_id = geo_base.lookup(location);
        if (region_id == UNKNOWN_GEO_ID) {
            std::cout << "unknown" << std::endl;
            continue;
        }

        std::cout << region_id << std::endl;
        bool const ret = geo_base.kv(region_id, [&] (char const *k, char const *v) {
            log_debug("k=\"%s\" v=\"%s\"", k, v);
        });

        if (!ret)
            log_warning("kv not found for %lu region_id", region_id);
    }

    return 0;
}
