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

static void show_geo_data(geo_base_t const &geo_base)
{
    geo_data::show(log_fd(), geo_base.geo_data());
}

static void show_max_edge_refs(geo_base_t const &geo_base, size_t regions_number)
{
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

    for (size_t i = 0; i < std::min(regions_number, regions.size()); ++i)
        log_info("(edge_refs_size) %s (%lu) = %.3f Mb", get_output(regions[i].second, geo_base),
            regions[i].second, regions[i].first / (1024.0 * 1024.0));
}

static uint64_t get_bits_number(uint64_t x)
{
    uint64_t number = 0;
    while (x) {
        ++number;
        x >>= 1;
    }
    return number;
}

static void show_possible_edge_refs_compression(geo_base_t const &geo_base)
{
    geo_data_t const &g = geo_base.geo_data();

    uint64_t size = 0;
    uint64_t possible_bits_size = 0;

    geo_base.each_polygon([&] (polygon_t const &polygon) {
        geo_base.each_part(polygon, [&] (part_t const &part, number_t const edge_refs_number) {
            number_t const edge_refs_offset = part.edge_refs_offset;
            size += edge_refs_number * sizeof(*geo_base.geo_data().edge_refs());

            ref_t min_edge_ref = g.edge_refs()[edge_refs_offset];
            for (number_t i = edge_refs_offset; i < edge_refs_offset + edge_refs_number; ++i)
                min_edge_ref = std::min(min_edge_ref, g.edge_refs()[i]);

            uint64_t bits_number = 0;
            for (number_t i = edge_refs_offset; i < edge_refs_offset + edge_refs_number; ++i)
                bits_number = std::max(bits_number, get_bits_number(g.edge_refs()[i] - min_edge_ref));

            possible_bits_size += bits_number * edge_refs_number;
        });
    });

    uint64_t possible_size = possible_bits_size / 8.0 + 1;

    log_info("(possible_edge_refs_size) %.3f Mb", possible_size / (1024.0 * 1024.0));
    log_info("(possible_edge_refs_compression) %.2f%%", possible_size * 100.0 / size);
}

int main(int argc, char *argv[])
{
    log_setup(STDERR_FILENO, LOG_LEVEL_DEBUG);

    if (argc < 2) {
        log_error("USAGE: geo-base-stat <geo-base.dat>");
        return -1;
    }

    int regions_number = 10;
    if (argc > 2)
        regions_number = atoi(argv[2]);

    geo_base_t geo_base(argv[1]);

    show_geo_data(geo_base);
    show_max_edge_refs(geo_base, regions_number);
    show_possible_edge_refs_compression(geo_base);

    return 0;
}
