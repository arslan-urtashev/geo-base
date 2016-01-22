// Copyright (c) 2015 Urtashev Arslan. All rights reserved.
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

#include <fcntl.h>
#include <gtest/gtest.h>

#include <geo_base/open_street_map/open_street_map.h>
#include <geo_base/open_street_map/parser.h>
#include <geo_base/open_street_map/reader.h>
#include <geo_base/open_street_map/simple_counter.h>
#include <geo_base/lib/fd_guard.h>
#include <geo_base/lib/file.h>
#include <geo_base/lib/file_stream.h>
#include <geo_base/lib/io_stream.h>
#include <geo_base/lib/memory.h>
#include <geo_base/lib/pool_allocator.h>

using namespace geo_base;
using namespace open_street_map;

static char const *OSM_PBF_TEST_FILE = "test/andorra-latest.osm.pbf";

TEST(open_street_map_parser, parse)
{
    pool_allocator_t allocator(16_mb);

    file_t file;
    file.read_open(OSM_PBF_TEST_FILE);
    file_input_stream_t input_stream(file.fd());

    reader_t reader(&input_stream);
    simple_counter_t count(&allocator);

    count.parse(&reader);

    EXPECT_EQ(123736ull, count.nodes_number());
    EXPECT_EQ(142ull, count.relations_number());
    EXPECT_EQ(5750ull, count.ways_number());

    EXPECT_EQ(count.nodes_processed() + count.dense_nodes_processed(), count.nodes_number());
    EXPECT_EQ(count.ways_processed(), count.ways_number());
    EXPECT_EQ(count.relations_processed(), count.relations_number());

    log_info("Parsed %lu relations, %lu ways and %lu nodes",
        count.relations_number(), count.ways_number(), count.nodes_number());
}

TEST(open_street_map_parser, pool_parser)
{
    std::vector<pool_allocator_t> allocators;
    std::vector<simple_counter_t> simple_counters;

    allocators.emplace_back(16_mb);
    allocators.emplace_back(16_mb);

    simple_counters.emplace_back(&allocators[0]);
    simple_counters.emplace_back(&allocators[1]);

    pool_parser_t parser;
    parser.parse(OSM_PBF_TEST_FILE, simple_counters);

    EXPECT_EQ(123736ull, simple_counters[0].nodes_number() + simple_counters[1].nodes_number());
    EXPECT_EQ(142ull, simple_counters[0].relations_number() + simple_counters[1].relations_number());
    EXPECT_EQ(5750ull, simple_counters[0].ways_number() + simple_counters[1].ways_number());

    log_info("Blocks processed 1 = %lu", simple_counters[0].blocks_processed());
    log_info("Blobks processed 2 = %lu", simple_counters[1].blocks_processed());
}
