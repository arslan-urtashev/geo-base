// Copyright (c) 2015, 2016 Urtashev Arslan. All rights reserved.
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

#include <geo_base/core/location.h>
#include <geo_base/core/point.h>
#include <geo_base/proto/region.pb.h>
#include <geo_base/util/allocator.h>
#include <geo_base/util/dynarray.h>

namespace geo_base {
namespace generator {

namespace {

inline double lon(location_t const &l)
{
	return l.lon;
}

inline double lat(location_t const &l)
{
	return l.lat;
}

inline double lon(proto::location_t const &l)
{
	return l.lon();
}

inline double lat(proto::location_t const &l)
{
	return l.lat();
}

} // namespace

template<typename a_t, typename b_t>
bool is_equal_locations(a_t const &a, b_t const &b)
{
	location_t a1(lon(a), lat(a));
	location_t b1(lon(b), lat(b));
	return point_t(a1) == point_t(b1);
}

class locations_converter_t {
public:
	explicit locations_converter_t(allocator_t *allocator)
		: allocator_(allocator)
	{
	}

	template<typename locations_t, typename callback_t>
	void each(locations_t const &raw_locations, callback_t callback)
	{
		dynarray_t<location_t> locations(raw_locations.size(), allocator_);
		for (auto const &l : raw_locations) {
			if (locations.empty() || !is_equal_locations(locations.back(), l))
				locations.push_back(location_t(lon(l), lat(l)));

			if (locations.size() > 3 && is_equal_locations(locations.front(), locations.back())) {
				locations.pop_back();
				callback(locations);
				locations.clear();
			}
		}

		if (locations.size() > 2)
			callback(locations);
	}

private:
	allocator_t *allocator_;
};

} // namespace generator
} // namespace geo_base
