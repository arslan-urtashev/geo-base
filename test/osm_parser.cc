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
#include <geo_base/util/fd_guard.h>
#include <geo_base/util/file.h>
#include <geo_base/util/file_stream.h>
#include <geo_base/util/io_stream.h>
#include <geo_base/util/memory.h>
#include <geo_base/util/pool_allocator.h>

using namespace geo_base;
using namespace open_street_map;

TEST(osm_parser, osm_parser)
{
	pool_allocator_t allocator(1_mb);
	
	file_t file("test/osm/andorra-latest.osm.pbf", file_t::READ_ONLY);
	file_input_stream_t input_stream(file.fd());

	reader_t reader(&input_stream);
	simple_counter_t count(&allocator);

	count.parse(&reader);

	EXPECT_EQ(123736ULL, count.nodes_count());
	EXPECT_EQ(5750ULL, count.ways_count());
	EXPECT_EQ(142ULL, count.relations_count());

	log_info("Parsed %lu relations, %lu ways and %lu nodes",
		count.relations_count(), count.ways_count(), count.nodes_count());
}

TEST(osm_parser, run_pool_parse)
{
	std::vector<pool_allocator_t> allocators;
	std::vector<simple_counter_t> simple_counters;

	allocators.emplace_back(1_mb);
	allocators.emplace_back(1_mb);

	simple_counters.emplace_back(&allocators[0]);
	simple_counters.emplace_back(&allocators[1]);

	run_pool_parse("test/osm/andorra-latest.osm.pbf", simple_counters);

	EXPECT_EQ(123736ULL, simple_counters[0].nodes_count() + simple_counters[1].nodes_count());
	EXPECT_EQ(5750ULL, simple_counters[0].ways_count() + simple_counters[1].ways_count());
	EXPECT_EQ(142ULL, simple_counters[0].relations_count() + simple_counters[1].relations_count());

	log_info("Blocks processed 1 = %lu", simple_counters[0].blocks_processed());
	log_info("Blobks processed 2 = %lu", simple_counters[1].blocks_processed());
}
