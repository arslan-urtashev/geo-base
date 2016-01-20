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

#include <iostream>
#include <geo_base/libproto/proto_reader.h>

using namespace geo_base;

int main(int argc, char *argv[])
{
    log_setup(STDERR_FILENO, LOG_LEVEL_DEBUG);

    if (argc < 2) {
        log_error("USAGE: write-proto <geo-base.pbf>");
        return -1;
    }

    proto_reader_t reader(argv[1]);

    if (argc == 2) {
        reader.each([&] (proto::region_t const &region) {
            std::string const debug_string = region.Utf8DebugString();
            std::cout << debug_string << std::endl;
        });
    } else {
        geo_id_t geo_id = strtoull(argv[2], nullptr, 10);
        reader.region(geo_id, [&] (proto::region_t const &region) {
            std::string const debug_string = region.Utf8DebugString();
            std::cout << debug_string << std::endl;
        });
    }

    return 0;
}
