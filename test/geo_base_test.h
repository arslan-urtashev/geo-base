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

#include <gmock/gmock.h>

#include <geo_base/edge.h>
#include <geo_base/geo_data.h>
#include <geo_base/util/log.h>

namespace geo_base {

class geo_data_test_t : public geo_data_t {
#define GEO_BASE_DEF_VAR(var_t, var) \
public: \
	virtual var_t const &var() const \
	{ \
		return var##_; \
	} \
protected: \
	var_t var##_;

#define GEO_BASE_DEF_ARR(arr_t, arr) \
public: \
	virtual arr_t const *arr() const { \
		return arr##_.data(); \
	} \
	virtual count_t arr##_count() const { \
		return arr##_.size(); \
	} \
protected: \
	std::vector<arr_t> arr##_;

	GEO_BASE_DEF_GEO_DATA

#undef GEO_BASE_DEF_VAR
#undef GEO_BASE_DEF_ARR
};

struct part_helper_t : public part_t {
	count_t edge_refs_count;

	bool contains(point_t const &point, geo_data_t const &geo_data) const
	{
		return part_t::contains(point, edge_refs_count, geo_data.edge_refs(), geo_data.edges(),
			geo_data.points());
	}
};

class geo_base_test_t : public geo_data_test_t, public ::testing::Test {
public:
	geo_base_test_t()
	{
		log_setup(STDERR_FILENO, LOG_LEVEL_DEBUG);
	}

	edge_t make_edge(point_t const &a, point_t const &b)
	{
		points_.push_back(a);
		points_.push_back(b);
		return edge_t(points_.size() - 2, points_.size() - 1);
	}

	part_helper_t make_part(coordinate_t coordinate, std::vector<edge_t> const &edges)
	{
		part_helper_t helper;
		helper.coordinate = coordinate;
		helper.edge_refs_offset = edge_refs_.size();

		for (edge_t const &e : edges) {
			edge_refs_.push_back(edges_.size());
			edges_.push_back(e);
		}

		helper.edge_refs_count = edge_refs_.size();

		std::sort(edge_refs_.begin() + helper.edge_refs_offset, edge_refs_.end(),
			[&] (ref_t const &a, ref_t const &b)
			{
				edge_t const &e1 = edges_[a];
				edge_t const &e2 = edges_[b];
				return e1.lower(e2, points_.data());
			}
		);

		return helper;
	}

	geo_data_test_t const &geo_data() const
	{
		geo_data_test_t const *data = dynamic_cast<geo_data_test_t const *>(this);
		assert(data != nullptr);
		return *data;
	}
};

}