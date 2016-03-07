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

#include <geo_base/generator/points_converter.h>
#include <geo_base/library/pool_allocator.h>
#include <test/geo_base_test.h>

using namespace geo_base;
using namespace generator;

struct points_converter_test_t : public geo_base_test_t {
};

TEST_F(points_converter_test_t, points_converter)
{
    pool_allocator_t allocator(1_mb);

    std::vector<point_t> const raw_points = {
        { 0, 0 },
        { 10, 0 },
        { 10, 10 },
    };

    points_converter_t converter(&allocator);
    dynarray_t<point_t> points = converter.convert(raw_points);

    ASSERT_TRUE(points[0] == point_t(0, 0));
    ASSERT_TRUE(points[1] == point_t(11, 0));
    ASSERT_TRUE(points[2] == point_t(10, 10));
}
