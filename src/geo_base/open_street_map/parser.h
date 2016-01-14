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

#pragma once

#include <thread>
#include <future>

#include <geo_base/location.h>
#include <geo_base/open_street_map/open_street_map.h>
#include <geo_base/open_street_map/proto/open_street_map.pb.h>
#include <geo_base/open_street_map/reader.h>
#include <geo_base/util/allocator.h>
#include <geo_base/util/dynarray.h>
#include <geo_base/util/file.h>
#include <geo_base/util/file_stream.h>
#include <geo_base/util/log.h>
#include <geo_base/util/stop_watch.h>

namespace geo_base {
namespace open_street_map {

class parser_t {
public:
	parser_t()
		: allocator_(nullptr)
		, blocks_processed_(0)
	{
	}
	
	explicit parser_t(allocator_t *allocator)
		: allocator_(allocator)
		, blocks_processed_(0)
	{
	}

	parser_t(parser_t &&p)
		: parser_t()
	{
		std::swap(allocator_, p.allocator_);
		std::swap(blocks_processed_, p.blocks_processed_);
	}

	void parse(reader_t *reader);

	size_t blocks_processed() const
	{
		return blocks_processed_;
	}

protected:
	virtual void process_node(geo_id_t, location_t const &, kvs_t const &)
	{
		// Do nothing.
	}

	virtual void process_way(geo_id_t, kvs_t const &, geo_ids_t const &)
	{
		// Do nothing.
	}

	virtual void process_relation(geo_id_t, kvs_t const &, references_t const &)
	{
		// Do nothing.
	}

	allocator_t *allocator_;

private:
	void process_basic_groups(proto::basic_block_t const &block);

	void process_dense_nodes(proto::dense_nodes_t const &nodes, proto::basic_block_t const &block);

	size_t blocks_processed_;

	parser_t(parser_t const &) = delete;
	parser_t &operator = (parser_t const &) = delete;
};

// Run parser_t::parse in different threads.
template<typename parser_t>
void run_pool_parse(reader_t *reader, std::vector<parser_t> &parsers)
{
	std::vector<std::thread> threads(parsers.size());
	for (size_t i = 0; i < threads.size(); ++i)
		threads[i] = std::thread([&parsers, &reader, i] () {
			parsers[i].parse(reader);
		});
	for (size_t i = 0; i < threads.size(); ++i)
		threads[i].join();
}

// Run parser_t::parse in different threads.
template<typename parser_t>
void run_pool_parse(char const *path, std::vector<parser_t> &parsers)
{
	log_info("Parse %s", path);

	stop_watch_t stop_watch;
	stop_watch.run();

	{
		file_t file;
		file.read_open(path);
		file_input_stream_t input_stream(file.fd());
		reader_t reader(&input_stream);
		run_pool_parse(&reader, parsers);
	}

	log_info("Parsed %s in %.3f seconds", path, stop_watch.get());
}

} // namespace open_street_map
} // namespace geo_base
