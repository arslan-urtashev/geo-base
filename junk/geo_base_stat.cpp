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
#include <unordered_map>
#include <vector>

#include <geo_base/core/geo_base.h>
#include <geo_base/core/geo_data/debug.h>
#include <geo_base/core/geo_data/map.h>
#include <geo_base/core/location.h>
#include <geo_base/lib/log.h>
#include <geo_base/lib/mem_file.h>

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

static void show_geo_data(geo_base_t const &geo_base)
{
    geo_data::show(log_fd(), geo_base.geo_data());
}

static void show_max_edge_refs(geo_base_t const &geo_base)
{
    static size_t const REGIONS_NUMBER = 7;

    std::unordered_map<geo_id_t, size_t> size;
    
    geo_base.each_polygon([&] (polygon_t const &polygon) {
        geo_base.each_part(polygon, [&] (part_t const &part, number_t const edge_refs_number) {
            size[polygon.region_id] += edge_refs_number * sizeof(*geo_base.geo_data().edge_refs());
        });
    });

    std::vector<std::pair<size_t, geo_id_t>> regions;
    for (auto const &p : size)
        regions.emplace_back(p.second, p.first);

    std::sort(regions.begin(), regions.end());
    std::reverse(regions.begin(), regions.end());

    for (size_t i = 0; i < std::min(REGIONS_NUMBER, regions.size()); ++i)
        log_info("(edge refs size) %s = %.3f Mb", get_output(regions[i].second, geo_base),
            regions[i].first / (1024.0 * 1024.0));
}

int main(int argc, char *argv[])
{
    log_setup(STDERR_FILENO, LOG_LEVEL_DEBUG);

    if (argc < 2) {
        log_error("USAGE: geo-base-stat <geo-base.dat>");
        return -1;
    }

    geo_base_t geo_base(argv[1]);

    show_geo_data(geo_base);
    show_max_edge_refs(geo_base);

    return 0;
}
