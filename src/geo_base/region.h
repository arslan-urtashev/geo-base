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

#include <geo_base/geo_base.h>
#include <geo_base/proto/region.pb.h>

namespace geo_base {

struct region_t {
	typedef uint64_t options_t;

	enum option_t : options_t {
		OPTION_ZERO                    = 0ull,
		OPTION_BOUNDARY_ADMINISTRATIVE = 1ull << proto::region_t::BOUNDARY_ADMINISTRATIVE,
		OPTION_PLACE_ISLAND            = 1ull << proto::region_t::PLACE_ISLAND,
		OPTION_PLACE_TOWN              = 1ull << proto::region_t::PLACE_TOWN,
		OPTION_PLACE_CITY              = 1ull << proto::region_t::PLACE_CITY,
		OPTION_PLACE_VILLAGE           = 1ull << proto::region_t::PLACE_VILLAGE,
		OPTION_PLACE_BOROUGH           = 1ull << proto::region_t::PLACE_BOROUGH,
		OPTION_PLACE_SUBURB            = 1ull << proto::region_t::PLACE_SUBURB,
	};

	geo_id_t region_id;
	count_t kvs_offset;
	count_t kvs_count;
	square_t square;
	options_t options;

	bool operator == (region_t const &r) const
	{
		return region_id == r.region_id;
	}

	bool operator < (region_t const &r) const
	{
		return region_id < r.region_id;
	}

	bool operator < (geo_id_t const &r) const
	{
		return region_id < r;
	}

	friend bool operator < (geo_id_t const &region_id, region_t const &r)
	{
		return region_id < r.region_id;
	}

	bool better(region_t const &r) const
	{
		return square < r.square;
	}
};

} // namespace geo_base
