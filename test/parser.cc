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

static char const *OSM_PBF_TEST_FILE = "test/andorra-latest.osm.pbf";

TEST(open_street_map_parser, parse)
{
	pool_allocator_t allocator(1_mb);
	
	file_t file;
	file.read_open(OSM_PBF_TEST_FILE);
	file_input_stream_t input_stream(file.fd());

	reader_t reader(&input_stream);
	simple_counter_t count(&allocator);

	count.parse(&reader);

	EXPECT_EQ(123736ULL, count.nodes_count());
	EXPECT_EQ(5750ULL, count.ways_count());
	EXPECT_EQ(142ULL, count.relations_count());

	EXPECT_EQ(count.nodes_processed() + count.dense_nodes_processed(), count.nodes_count());
	EXPECT_EQ(count.ways_processed(), count.ways_count());
	EXPECT_EQ(count.relations_processed(), count.relations_count());

	log_info("Parsed %lu relations, %lu ways and %lu nodes",
		count.relations_count(), count.ways_count(), count.nodes_count());
}

TEST(open_street_map_parser, run_pool_parse)
{
	std::vector<pool_allocator_t> allocators;
	std::vector<simple_counter_t> simple_counters;

	allocators.emplace_back(1_mb);
	allocators.emplace_back(1_mb);

	simple_counters.emplace_back(&allocators[0]);
	simple_counters.emplace_back(&allocators[1]);

	run_pool_parse(OSM_PBF_TEST_FILE, simple_counters);

	EXPECT_EQ(123736ULL, simple_counters[0].nodes_count() + simple_counters[1].nodes_count());
	EXPECT_EQ(5750ULL, simple_counters[0].ways_count() + simple_counters[1].ways_count());
	EXPECT_EQ(142ULL, simple_counters[0].relations_count() + simple_counters[1].relations_count());

	log_info("Blocks processed 1 = %lu", simple_counters[0].blocks_processed());
	log_info("Blobks processed 2 = %lu", simple_counters[1].blocks_processed());
}

class hash_counter_t : public parser_t {
public:
	static uint64_t const POWER = 719ull;

	hash_counter_t(allocator_t *allocator)
		: parser_t(allocator)
		, hash_(0)
		, power_(1)
	{
	}

	static uint64_t kvs_hash(kvs_t const &kvs)
	{
		static uint64_t KVS_POWER = 117ull;
		uint64_t hash = 0, power = 1;
		for (kv_t const &kv : kvs) {
			for (char const *c = kv.k; *c; ++c) {
				hash += ((uint64_t) *c) * power;
				power *= KVS_POWER;
			}
			for (char const *c = kv.v; *c; ++c) {
				hash += ((uint64_t) *c) * power;
				power *= KVS_POWER;
			}
		}
		return power;
	}

	static uint64_t ids_hash(geo_ids_t const &ids)
	{
		static uint64_t IDS_POWER = 317ull;
		uint64_t hash = 0, power = 1;
		for (geo_id_t const &id : ids) {
			hash += id * power;
			power *= IDS_POWER;
		}
		return hash;
	}

	static uint64_t refs_hash(references_t const &refs)
	{
		static uint64_t REF_POWER = 319ull;
		uint64_t hash = 0, power = 1;
		for (reference_t const &ref : refs) {
			hash += ref.geo_id * power;
			power *= REF_POWER;
			hash += ref.type * power;
			power *= REF_POWER;
			for (char const *c = ref.role; *c; ++c) {
				hash += ((uint64_t) *c) * power;
				power *= REF_POWER;
			}
		}
		return hash;
	}

	void process_node(geo_id_t geo_id, location_t const &location, kvs_t const &kvs) override
	{
		uint64_t node_hash = 0;
		node_hash += geo_id * 777ull + location.lat * 333ull + location.lon * 123ull;
		node_hash += kvs_hash(kvs) * 765ull;
		hash_ += node_hash * power_;
		power_ *= POWER;
	}

	void process_way(geo_id_t geo_id, kvs_t const &kvs, geo_ids_t const &ids) override
	{
		uint64_t way_hash = geo_id * 723ull + kvs_hash(kvs) * 111ull + ids_hash(ids) * 567ull;
		hash_ += way_hash * power_;
		power_ *= POWER;
	}

	void process_relation(geo_id_t geo_id, kvs_t const &kvs, references_t const &refs) override
	{
		uint64_t relation_hash = geo_id * 237ull + kvs_hash(kvs) * 222ull + refs_hash(refs) * 999ull;
		hash_ += relation_hash * power_;
		power_ *= POWER;
	}

	uint64_t hash() const
	{
		return hash_;
	}

	uint64_t power() const
	{
		return power_;
	}

private:
	uint64_t hash_;
	uint64_t power_;
};

TEST(open_street_map_parser, double_parse)
{
	pool_allocator_t allocator(1_mb);
	uint64_t hash = 0, power = 0;

	{
		file_t file;
		file.read_open(OSM_PBF_TEST_FILE);
		file_input_stream_t input_stream(file.fd());

		reader_t reader(&input_stream);
		hash_counter_t counter(&allocator);

		counter.parse(&reader);
		
		hash = counter.hash();
		power = counter.power();
	}

	ASSERT_NE(0ull, hash);
	ASSERT_NE(0ull, power);

	log_info("hash = %llu, power = %llu", hash, power);

	{
		file_t file;
		file.read_open(OSM_PBF_TEST_FILE);
		file_input_stream_t input_stream(file.fd());

		reader_t reader(&input_stream);
		hash_counter_t counter(&allocator);

		counter.parse(&reader);

		ASSERT_EQ(hash, counter.hash());
		ASSERT_EQ(power, counter.power());
	}
}
