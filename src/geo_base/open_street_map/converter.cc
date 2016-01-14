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

#include <geo_base/open_street_map/converter.h>
#include <geo_base/polygon.h>
#include <geo_base/region.h>

namespace geo_base {
namespace open_street_map {

static bool eq(char const *a, char const *b)
{
	return !strcmp(a, b);
}

static region_t::options_t get_region_options(kvs_t const &kvs)
{
	region_t::options_t options = 0;

	for (kv_t const &kv : kvs) {
		char const *k = kv.k;
		char const *v = kv.v;

		if (eq(k, "boundary") && eq(v, "administrative"))
			options |= region_t::OPTION_BOUNDARY_ADMINISTRATIVE;
		else if (eq(k, "place") && eq(v, "island"))
			options |= region_t::OPTION_PLACE_ISLAND;
		else if (eq(k, "place") && eq(v, "town"))
			options |= region_t::OPTION_PLACE_TOWN;
		else if (eq(k, "place") && eq(v, "city"))
			options |= region_t::OPTION_PLACE_CITY;
		else if (eq(k, "place") && eq(v, "village"))
			options |= region_t::OPTION_PLACE_VILLAGE;
		else if (eq(k, "place") && eq(v, "borough"))
			options |= region_t::OPTION_PLACE_BOROUGH;
		else if (eq(k, "place") && eq(v, "suburb"))
			options |= region_t::OPTION_PLACE_SUBURB;
	}

	return options;
}

static polygon_t::type_t get_polygon_type(char const *role)
{
	if (!role)
		return polygon_t::TYPE_OUTER;
	if (eq(role, "outer"))
		return polygon_t::TYPE_OUTER;
	if (eq(role, "inner"))
		return polygon_t::TYPE_INNER;
	return polygon_t::TYPE_UNKNOWN;
}

static bool is_boundary(kvs_t const &kvs)
{
	if (get_region_options(kvs) == 0)
		return false;
	for (kv_t const &kv : kvs)
		if (strstr(kv.k, "name") == kv.k)
			return true;
	return false;
}

static bool check_role(char const *role)
{
	polygon_t::type_t type = get_polygon_type(role);
	return type == polygon_t::TYPE_INNER || type == polygon_t::TYPE_OUTER;
}

static bool is_way_reference(reference_t const &r)
{
	return r.type == reference_t::TYPE_WAY && check_role(r.role);
}

void grep_boundary_ways_t::process_way(geo_id_t geo_id, kvs_t const &kvs, geo_ids_t const &refs)
{
	if (is_boundary(kvs) && refs.front() == refs.back())
		ways_.insert(geo_id);
}

void grep_boundary_ways_t::process_relation(geo_id_t, kvs_t const &kvs, references_t const &refs)
{
	if (is_boundary(kvs))
		for (reference_t const &ref : refs)
			if (is_way_reference(ref))
				ways_.insert(ref.geo_id);
}


} // namespace open_street_map
} // namespace geo_base
