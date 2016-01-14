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

#include <geo_base/open_street_map/parser.h>

namespace geo_base {
namespace open_street_map {

class simple_counter_t : public parser_t {
public:
	simple_counter_t(allocator_t *allocator)
		: parser_t(allocator)
		, nodes_count_(0)
		, ways_count_(0)
		, relations_count_(0)
	{
	}

	simple_counter_t(simple_counter_t &&c)
		: parser_t(std::forward<parser_t>(c))
		, nodes_count_(0)
		, ways_count_(0)
		, relations_count_(0)
	{
		std::swap(nodes_count_, c.nodes_count_);
		std::swap(ways_count_, c.ways_count_);
		std::swap(relations_count_, c.relations_count_);
	}

	void process_node(geo_id_t, location_t const &, kvs_t const &) override
	{
		++nodes_count_;
	}

	void process_way(geo_id_t, kvs_t const &, geo_ids_t const &) override
	{
		++ways_count_;
	}

	void process_relation(geo_id_t, kvs_t const &, references_t const &) override
	{
		++relations_count_;
	}

	size_t nodes_count() const
	{
		return nodes_count_;
	}

	size_t ways_count() const
	{
		return ways_count_;
	}

	size_t relations_count() const
	{
		return relations_count_;
	}

private:
	size_t nodes_count_;
	size_t ways_count_;
	size_t relations_count_;

	simple_counter_t(simple_counter_t const &) = delete;
	simple_counter_t &operator = (simple_counter_t const) = delete;
};

} // namespace open_street_map
} // namespace geo_base
