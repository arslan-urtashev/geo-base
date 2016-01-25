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
#include <geo_base/open_street_map/converter.h>
#include <geo_base/libproto/proto_reader.h>
#include <geo_base/lib/pool_allocator.h>

#include "geo_base_test.h"

using namespace geo_base;
using namespace open_street_map;

class open_street_map_convert_t : public test_t {
};

TEST_F(open_street_map_convert_t, convert)
{
    ASSERT_NO_THROW(run_pool_convert("test/andorra-latest.osm.pbf", "andorra-latest.pbf", 2));

    proto_reader_t reader("andorra-latest.pbf");

    size_t regions_number = 0;
    size_t polygons_number = 0;

    reader.each([&] (::geo_base::proto::region_t const &r) {
        ++regions_number;
        polygons_number += r.polygons_size();
    });

    EXPECT_EQ(148ul, regions_number);
    EXPECT_EQ(153ul, polygons_number);

    remove("andorra-latest.pbf");
}
