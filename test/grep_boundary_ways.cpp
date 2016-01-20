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
#include <geo_base/lib/file.h>
#include <geo_base/lib/file_stream.h>
#include <geo_base/lib/pool_allocator.h>

using namespace geo_base;
using namespace open_street_map;

TEST(grep_boundary_ways_t, check_boundary_ways)
{
    pool_allocator_t allocator(16_mb);

    file_t file;
    file.read_open("test/andorra-latest.osm.pbf");
    file_input_stream_t stream(file.fd());

    reader_t reader(&stream);
    grep_boundary_ways_t grep(&allocator);

    grep.parse(&reader);

    log_info("Found %lu boundary ways", grep.ways().size());

    ASSERT_NE(0u, grep.ways().size());
    ASSERT_EQ(3411u, grep.ways().size());
}
