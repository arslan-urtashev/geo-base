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

#include <test/geo_base_test.h>

using namespace geo_base;

class part_test_t : public geo_base_test_t {
};

TEST_F(part_test_t, two_edges)
{
    part_helper_t part = make_part(0, {
        make_edge(point_t(0, 0), point_t(3, 0)),
        make_edge(point_t(0, 3), point_t(5, 5))
    });

    EXPECT_EQ(0, part.coordinate);
    EXPECT_TRUE(part.contains(point_t(1, 1), geo_data()));
}

TEST_F(part_test_t, contains)
{
    part_helper_t part = make_part(0, {
        make_edge(point_t(0, 0), point_t(3, 0)),
        make_edge(point_t(0, 3), point_t(3, 0)),
        make_edge(point_t(-1, 10), point_t(10, 10)),
        make_edge(point_t(-1, 10), point_t(10, 100))
    });

    EXPECT_TRUE(part.contains(point_t(1, 1), geo_data()));
    EXPECT_TRUE(part.contains(point_t(3, 0), geo_data()));
    EXPECT_TRUE(part.contains(point_t(0, 11), geo_data()));
    EXPECT_TRUE(part.contains(point_t(0, 3), geo_data()));

    EXPECT_FALSE(part.contains(point_t(2, 5), geo_data()));
    EXPECT_FALSE(part.contains(point_t(2, -1), geo_data()));
    EXPECT_FALSE(part.contains(point_t(2, 101), geo_data()));
}
