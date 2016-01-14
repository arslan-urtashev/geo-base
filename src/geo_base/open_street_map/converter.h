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

#include <unordered_set>

namespace geo_base {
namespace open_street_map {

class grep_boundary_ways_t : public parser_t {
public:
	grep_boundary_ways_t(allocator_t *allocator)
		: parser_t(allocator)
	{
	}

	void process_way(geo_id_t, kvs_t const &, geo_ids_t const &) override;

	void process_relation(geo_id_t, kvs_t const &, references_t const &) override;

	std::unordered_set<geo_id_t> const &ways() const
	{
		return ways_;
	}

private:
	std::unordered_set<geo_id_t> ways_;
};

class converter_t {
};

} // namespace open_street_map
} // namespace geo_base
