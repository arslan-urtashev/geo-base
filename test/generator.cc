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

#include <geo_base/generator/generator.h>
#include <geo_base/generator/geo_data.h>
#include <geo_base/util/pool_allocator.h>


namespace geo_base {
namespace generator {

class geo_data_test_t : public geo_data_t {
#define GEO_BASE_DEF_VAR(var_t, var) \
public: \
	virtual var_t const &var() const \
	{ \
		return var##_; \
	} \
	virtual void set_##var(var_t const &v) \
	{ \
		var##_ = v; \
	} \
private: \
	var_t var##_;

#define GEO_BASE_DEF_ARR(arr_t, arr) \
public: \
	virtual arr_t const *arr() const \
	{ \
		return arr##_.data(); \
	} \
	virtual arr_t *mut_##arr() \
	{ \
		return arr##_.data(); \
	} \
	virtual count_t arr##_count() const \
	{ \
		return arr##_.size(); \
	} \
	virtual void arr##_append(arr_t const &a) \
	{ \
		arr##_.push_back(a); \
	} \
private: \
	std::vector<arr_t> arr##_;

	GEO_BASE_DEF_GEO_DATA

#undef GEO_BASE_DEF_VAR
#undef GEO_BASE_DEF_ARR

	virtual ref_t insert(point_t const &p)
	{
		points_.push_back(p);
		return points_.size() - 1;
	}

	virtual ref_t insert(edge_t const &e)
	{
		edges_.push_back(e);
		return edges_.size() - 1;
	}
};

} // namespace generator
} // namespace geo_base

using namespace geo_base;

class generator_test_t : public test_t {
};

TEST_F(generator_test_t, generator_test)
{
	pool_allocator_t allocator(1024 * 1024 * 1024);

	generator::geo_data_test_t geo_data;
	generator::generator_t generator(&geo_data, &allocator);

	dynarray_t<point_t> points(4, &allocator);
	points.push_back(point_t(to_coordinate(0), to_coordinate(0)));
	points.push_back(point_t(to_coordinate(10), to_coordinate(0)));
	points.push_back(point_t(to_coordinate(10), to_coordinate(0)));
	points.push_back(point_t(to_coordinate(10), to_coordinate(10)));

	generator.update(123, 123, points, false);

	generator.fini();

	EXPECT_EQ(123ull, geo_data.lookup(location_t(5, 5)));
	EXPECT_EQ(UNKNOWN_GEO_ID, geo_data.lookup(location_t(-1, -1)));
	EXPECT_EQ(UNKNOWN_GEO_ID, geo_data.lookup(location_t(0, 3)));
	EXPECT_EQ(123ull, geo_data.lookup(location_t(4, 0)));
	EXPECT_EQ(123ull, geo_data.lookup(location_t(5, 5)));
}
