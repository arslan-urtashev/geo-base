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

#include <geo_base/location.h>
#include <geo_base/open_street_map/open_street_map.h>
#include <geo_base/open_street_map/proto/open_street_map.pb.h>
#include <geo_base/open_street_map/reader.h>
#include <geo_base/util/allocator.h>
#include <geo_base/util/dynarray.h>

namespace geo_base {
namespace open_street_map {

class parser_t {
public:
	parser_t(allocator_t *allocator)
		: allocator_(allocator)
	{
	}

	void parse(reader_t *reader);

protected:
	virtual void process_node(geo_id_t geo_id, location_t const &location,
		dynarray_t<kv_t> const &kvs) = 0;

	virtual void process_way(geo_id_t geo_id, dynarray_t<kv_t> const &kvs,
		dynarray_t<geo_id_t> const &references) = 0;

	virtual void process_relation(geo_id_t geo_id, dynarray_t<kv_t> const &kvs,
		dynarray_t<reference_t> const &references) = 0;

	allocator_t *allocator_;

private:
	void process_basic_groups(proto::basic_block_t const &block);

	void process_dense_nodes(proto::dense_nodes_t const &nodes, proto::basic_block_t const &block);
};

// Run parser_t::parse in different threads.
void run_pool_parse(char const *path, parser_t *parsers, size_t parsers_count);

} // namespace open_street_map
} // namespace geo_base
