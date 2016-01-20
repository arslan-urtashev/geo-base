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

#include <geo_base/generator/locations_converter.h>
#include <geo_base/library/pool_allocator.h>

#include "geo_base_test.h"

using namespace geo_base;
using namespace geo_base::generator;

struct location_converter_t : public geo_base_test_t {
};

TEST_F(location_converter_t, location_converter)
{
    std::vector<location_t> raw_locations = {
        { 0, 0 },
        { 1, 1 },
        { 2, 2 },
        { 3, 3 },
        { 0, 0 },
        { 5, 5 },
        { 6, 6 },
        { 7, 7 },
        { 5, 5 },
        { 10, 10 },
        { 11, 11 },
        { 12, 12 }
    };

    std::vector<size_t> offsets = {
        0, 5, 9
    };

    std::vector<size_t> sizes = {
        4, 3, 3
    };

    pool_allocator_t allocator(1024);
    locations_converter_t converter(&allocator);

    size_t callbacks_count = 0;

    converter.each(raw_locations, [&] (dynarray_t<location_t> const &locations) {
        ASSERT_EQ(sizes[callbacks_count], locations.size());

        size_t offset = offsets[callbacks_count];
        size_t size = sizes[callbacks_count];

        for (size_t i = offset; i < offset + size; ++i)
            ASSERT_TRUE(is_equal_locations(raw_locations[i], locations[i - offset]));

        ++callbacks_count;
    });

    ASSERT_EQ(3ULL, callbacks_count);
}
