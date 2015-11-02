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

#include "geo_base_test.h"

#include <fcntl.h>
#include <geo_base/util/fd_guard.h>
#include <geo_base/util/file.h>
#include <geo_base/util/file_stream.h>
#include <geo_base/util/io_stream.h>
#include <geo_base/util/pool_allocator.h>
#include <geo_base/util/memory.h>
#include <tool/osm/osm.h>
#include <tool/osm/osm_parser.h>
#include <tool/osm/osm_reader.h>

using namespace geo_base;
using namespace geo_base::tool;

struct osm_counter_t : public osm_parser_t {
	osm_counter_t(allocator_t *allocator)
		: osm_parser_t(allocator)
		, nodes_amount(0)
		, ways_amount(0)
		, relations_amount(0)
	{
	}

	virtual void process_node(osm_id_t, osm_location_t const &, dynarray_t<osm_tag_t> const &)
	{
		++nodes_amount;
	}

	virtual void process_way(osm_id_t, dynarray_t<osm_tag_t> const &, dynarray_t<osm_id_t> const &)
	{
		++ways_amount;
	}

	virtual void process_relation(osm_id_t, dynarray_t<osm_tag_t> const &,
		dynarray_t<osm_reference_t> const &)
	{
		++relations_amount;
	}

	size_t nodes_amount;
	size_t ways_amount;
	size_t relations_amount;
};

TEST(osm_parser, osm_parser)
{
	pool_allocator_t allocator(1_mb);
	
	file_t file("test/osm/andorra-latest.osm.pbf", file_t::READ_ONLY);
	file_input_stream_t input_stream(file.fd());

	osm_reader_t reader(&input_stream);
	osm_counter_t amount(&allocator);

	amount.parse(&reader);

	EXPECT_EQ(123736ULL, amount.nodes_amount);
	EXPECT_EQ(5750ULL, amount.ways_amount);
	EXPECT_EQ(142ULL, amount.relations_amount);

	log_info("Parsed %lu relations, %lu ways and %lu nodes",
		amount.relations_amount, amount.ways_amount, amount.nodes_amount);
}
