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

#include <gmock/gmock.h>
#include <geo_base/util/algo.h>
#include <vector>

using namespace geo_base;

TEST(lower_bound, lower_bound)
{
    std::vector<int> const a = {1, 2, 3, 3, 5, 10, 10, 10, 22, 33};

    ASSERT_EQ(a.data() + 2, lower_bound(a.data(), a.size(), 3));
    ASSERT_EQ(a.data() + 5, lower_bound(a.data(), a.size(), 10));
    ASSERT_EQ(a.data() + 9, lower_bound(a.data(), a.size(), 23));

    ASSERT_EQ(a.data() + 1, lower_bound(a.data(), a.size(), 2));
    ASSERT_EQ(a.data() + 4, lower_bound(a.data(), a.size(), 4));
}

TEST(find, find)
{
    std::vector<int> const a = {1, 2, 3, 3, 5, 10, 10, 10, 22, 33};

    ASSERT_EQ(a.data() + 2, find(a.data(), a.size(), 3));
    ASSERT_EQ(a.data() + 5, find(a.data(), a.size(), 10));
    ASSERT_EQ(a.data() + 1, find(a.data(), a.size(), 2));

    ASSERT_EQ(nullptr, find(a.data(), a.size(), 23));
    ASSERT_EQ(nullptr, find(a.data(), a.size(), 4));
}
