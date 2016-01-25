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

class edge_test_t : public geo_base_test_t {
};

TEST_F(edge_test_t, default_constructor)
{
    char buf[sizeof(edge_t)];
    for (size_t i = 0; i < sizeof(edge_t); ++i)
        buf[i] = i + 1;

    edge_t *e = ::new ((void *) buf) edge_t();

    ASSERT_EQ(0u, e->beg);
    ASSERT_EQ(0u, e->end);
}

TEST_F(edge_test_t, assignment_contructor)
{
    edge_t a(10, 22);
    edge_t b(a);

    ASSERT_EQ(a.beg, b.beg);
    ASSERT_EQ(a.end, b.end);
}

TEST_F(edge_test_t, assignment_operator)
{
    edge_t a(10, 22);

    edge_t b;
    b = a;

    ASSERT_EQ(a.beg, b.beg);
    ASSERT_EQ(a.end, b.end);
}

TEST_F(edge_test_t, equals_operator)
{
    edge_t a(10, 22), b(10, 22);

    ASSERT_EQ(a, b);
}

TEST_F(edge_test_t, not_equals_operator)
{
    edge_t a(10, 22), b(11, 22), c(10, 23), d(11, 23);

    ASSERT_NE(a, b);
    ASSERT_NE(a, c);
    ASSERT_NE(a, d);
}

TEST_F(edge_test_t, less_operator)
{
    edge_t a(10, 22), b(9, 22), c(10, 21);

    ASSERT_FALSE(a < b);
    ASSERT_FALSE(a < c);

    ASSERT_TRUE(b < a);
    ASSERT_TRUE(c < a);
}

TEST_F(edge_test_t, contains_point)
{
    edge_t a = make_edge(point_t(0, 0), point_t(2, 2));

    EXPECT_TRUE(a.contains(point_t(0, 0), points()));
    EXPECT_TRUE(a.contains(point_t(1, 1), points()));
    EXPECT_TRUE(a.contains(point_t(2, 2), points()));

    EXPECT_FALSE(a.contains(point_t(-1, -1), points()));
    EXPECT_FALSE(a.contains(point_t(3, 3), points()));
    EXPECT_FALSE(a.contains(point_t(2, 1), points()));
}

TEST_F(edge_test_t, edge_lower)
{
    edge_t a = make_edge(point_t(0, 0), point_t(2, 1));
    edge_t b = make_edge(point_t(0, 0), point_t(2, 2));
    edge_t c = make_edge(point_t(1, 3), point_t(5, 5));
    edge_t d = make_edge(point_t(0, 4), point_t(5, 5));

    EXPECT_TRUE(a.lower(b, points()));
    EXPECT_TRUE(a.lower(c, points()));
    EXPECT_TRUE(a.lower(d, points()));
    EXPECT_TRUE(b.lower(c, points()));
    EXPECT_TRUE(b.lower(d, points()));
    EXPECT_TRUE(c.lower(d, points()));

    EXPECT_FALSE(d.lower(a, points()));
    EXPECT_FALSE(d.lower(b, points()));
    EXPECT_FALSE(d.lower(c, points()));
    EXPECT_FALSE(c.lower(b, points()));
    EXPECT_FALSE(c.lower(a, points()));
    EXPECT_FALSE(b.lower(a, points()));

    edge_t a1 = make_edge(point_t(0, 2), point_t(3, 3));
    edge_t a2 = make_edge(point_t(0, 0), point_t(1, 2));
    edge_t a3 = make_edge(point_t(0, 0), point_t(3, 3));

    EXPECT_TRUE(a3.lower(a2, points()));
    EXPECT_TRUE(a3.lower(a1, points()));
    EXPECT_TRUE(a2.lower(a1, points()));

    EXPECT_FALSE(a2.lower(a3, points()));
    EXPECT_FALSE(a1.lower(a3, points()));
    EXPECT_FALSE(a1.lower(a2, points()));

    edge_t a4 = make_edge(point_t(2, 5), point_t(3, 7));
    edge_t a5 = make_edge(point_t(2, 5), point_t(4, 6));
    edge_t a6 = make_edge(point_t(2, 4), point_t(4, 6));
    edge_t a11 = make_edge(point_t(3, 7), point_t(10, 5));

    EXPECT_TRUE(a6.lower(a5, points()));
    EXPECT_TRUE(a6.lower(a4, points()));
    EXPECT_TRUE(a6.lower(a11, points()));
    EXPECT_TRUE(a5.lower(a4, points()));
    EXPECT_TRUE(a5.lower(a11, points()));

    EXPECT_FALSE(a5.lower(a6, points()));
    EXPECT_FALSE(a4.lower(a6, points()));
    EXPECT_FALSE(a11.lower(a6, points()));
    EXPECT_FALSE(a4.lower(a5, points()));
    EXPECT_FALSE(a11.lower(a5, points()));

    edge_t a7 = make_edge(point_t(4, 4), point_t(5, 5));
    edge_t a8 = make_edge(point_t(5, 5), point_t(6, 4));
    edge_t a9 = make_edge(point_t(5, 4), point_t(6, 3));
    edge_t a10 = make_edge(point_t(4, 4), point_t(6, 3));

    EXPECT_TRUE(a10.lower(a9, points()));
    EXPECT_TRUE(a10.lower(a7, points()));
    EXPECT_TRUE(a10.lower(a8, points()));
    EXPECT_TRUE(a9.lower(a8, points()));

    EXPECT_FALSE(a9.lower(a10, points()));
    EXPECT_FALSE(a7.lower(a10, points()));
    EXPECT_FALSE(a8.lower(a10, points()));
    EXPECT_FALSE(a8.lower(a9, points()));
}
