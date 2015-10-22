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

#pragma once

#include "osm.h"
#include "osm_reader.h"

#include <geo_base/util/dynarray.h>

namespace geo_base {
namespace tool {

class osm_parser_t {
public:
	osm_parser_t(allocator_t *allocator)
		: allocator_(allocator)
	{
	}

	void parse(osm_reader_t *reader);

	void process_primitive_groups(osmpbf::primitive_block_t const &block);

	virtual void process_node(osm_id_t osm_id, osm_location_t const &location,
		dynarray_t<osm_tag_t> const &tags) = 0;

	virtual void process_way(osm_id_t osm_id, dynarray_t<osm_tag_t> const &tags,
		dynarray_t<osm_id_t> const &references) = 0;

	virtual void process_relation(osm_id_t osm_id, dynarray_t<osm_tag_t> const &tags,
		dynarray_t<osm_reference_t> const &references) = 0;

private:
	allocator_t *allocator_;
};

} // namespace tool
} // namespace geo_base
