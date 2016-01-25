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

#include <geo_base/lib/dynarray.h>
#include <geo_base/lib/memory.h>
#include <geo_base/lib/pool_allocator.h>
#include <test/geo_base_test.h>

using namespace geo_base;

struct pool_allocator_test_t : public geo_base_test_t {
};

TEST_F(pool_allocator_test_t, pool_allocator)
{
    pool_allocator_t pool_allocator(8_kb);

    {
        dynarray_t<int> a;

        {
            dynarray_t<long long> array(100, &pool_allocator);
            for (int i = 0; i < 100; ++i)
                array.push_back(100 - i);

            a = dynarray_t<int>(10, &pool_allocator);

            size_t expected_memory_size = 0;
            expected_memory_size += align_memory(sizeof(int) * 10);
            expected_memory_size += align_memory(sizeof(long long) * 100);
            expected_memory_size += 2 * align_memory(sizeof(size_t));

            ASSERT_EQ(expected_memory_size, pool_allocator.total_allocated_size());

            for (int i = 0; i < 100; ++i)
                ASSERT_EQ(100 - i, array[i]);

            std::sort(array.begin(), array.end());
            for (int i = 0; i < 100; ++i)
                ASSERT_EQ(i + 1, array[i]);
        }

        for (int i = 0; i < 10; ++i)
            a.push_back(i);

        for (int i = 0; i < 10; ++i) {
            dynarray_t<int> b(10, &pool_allocator);
            for (int j = 0; j < 10; ++j)
                b.push_back(i + j);

            for (int j = 0; j < 10; ++j)
                ASSERT_EQ(b[j], i + j);
        }

        for (int i = 0; i < 10; ++i)
            ASSERT_EQ(i, a[i]);
    }

    ASSERT_EQ(0ULL, pool_allocator.total_allocated_size());
}

TEST_F(pool_allocator_test_t, alignment)
{
    pool_allocator_t pool_allocator(8_kb);

    dynarray_t<long long> a(100, &pool_allocator);
    ASSERT_TRUE(is_aligned_memory(a.data()));

    dynarray_t<int> b(100, &pool_allocator);
    ASSERT_TRUE(is_aligned_memory(b.data()));

    dynarray_t<short> c(23, &pool_allocator);
    ASSERT_TRUE(is_aligned_memory(c.data()));

    dynarray_t<double> d(77, &pool_allocator);
    ASSERT_TRUE(is_aligned_memory(d.data()));
}
