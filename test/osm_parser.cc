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
#include <geo_base/util/fd_guard.h>
#include <geo_base/util/file.h>
#include <geo_base/util/file_stream.h>
#include <geo_base/util/io_stream.h>
#include <geo_base/util/memory.h>
#include <geo_base/util/pool_allocator.h>

using namespace geo_base;
using namespace open_street_map;

struct counter_t : public parser_t {
	explicit counter_t(allocator_t *allocator)
		: parser_t(allocator)
		, nodes_amount(0)
		, ways_amount(0)
		, relations_amount(0)
	{
	}

	counter_t(counter_t &&c)
		: parser_t(std::forward<parser_t>(c))
		, nodes_amount(0)
		, ways_amount(0)
		, relations_amount(0)
	{
		std::swap(nodes_amount, c.nodes_amount);
		std::swap(ways_amount, c.ways_amount);
		std::swap(relations_amount, c.relations_amount);
	}

	void process_node(geo_id_t, location_t const &,
		dynarray_t<kv_t> const &) override
	{
		++nodes_amount;
	}

	void process_way(geo_id_t, dynarray_t<kv_t> const &,
		dynarray_t<geo_id_t> const &) override
	{
		++ways_amount;
	}

	void process_relation(geo_id_t, dynarray_t<kv_t> const &,
		dynarray_t<reference_t> const &) override
	{
		++relations_amount;
	}

	size_t nodes_amount;
	size_t ways_amount;
	size_t relations_amount;

	counter_t(counter_t const &) = delete;
	counter_t &operator = (counter_t const) = delete;
};

TEST(osm_parser, osm_parser)
{
	pool_allocator_t allocator(1_mb);
	
	file_t file("test/osm/andorra-latest.osm.pbf", file_t::READ_ONLY);
	file_input_stream_t input_stream(file.fd());

	reader_t reader(&input_stream);
	counter_t amount(&allocator);

	amount.parse(&reader);

	EXPECT_EQ(123736ULL, amount.nodes_amount);
	EXPECT_EQ(5750ULL, amount.ways_amount);
	EXPECT_EQ(142ULL, amount.relations_amount);

	log_info("Parsed %lu relations, %lu ways and %lu nodes",
		amount.relations_amount, amount.ways_amount, amount.nodes_amount);
}

TEST(osm_parser, run_pool_parse)
{
	std::vector<pool_allocator_t> allocators;
	std::vector<counter_t> counters;

	allocators.emplace_back(1_mb);
	allocators.emplace_back(1_mb);

	counters.emplace_back(&allocators[0]);
	counters.emplace_back(&allocators[1]);

	run_pool_parse("test/osm/andorra-latest.osm.pbf", counters);

	EXPECT_EQ(123736ULL, counters[0].nodes_amount + counters[1].nodes_amount);
	EXPECT_EQ(5750ULL, counters[0].ways_amount + counters[1].ways_amount);
	EXPECT_EQ(142ULL, counters[0].relations_amount + counters[1].relations_amount);

	log_info("Blocks processed 1 = %lu", counters[0].blocks_processed());
	log_info("Blobks processed 2 = %lu", counters[1].blocks_processed());
}
