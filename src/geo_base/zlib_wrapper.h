// Copyright (c) 2015 Urtashev Arslan. All rights reserved.
// Contacts: <urtashev@gmail.com>
//   
// Permission is hereby granted, free of charge, to any person obtaining
// a copy of this software and associated documentation files (the
// "Software"), to deal in the Software without restriction, including
// without limitation the rights to use, copy, modify, merge, publish,
// distribute, sublicense, and/or sell copies of the Software, and to
// permit persons to whom the Software is furnished to do so, subject to
// the following conditions:
//           
//   The above copyright notice and this permission notice shall be included
//   in all copies or substantial portions of the Software.
//              
// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,
// EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF
// MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.
// IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY
// CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT,
// TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE
// SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.

#ifndef GEO_BASE_ZLIB_WRAPPER_H_
#define GEO_BASE_ZLIB_WRAPPER_H_

#include "exception.h"

#include "zlib/zlib.h"

namespace geo_base {

class ZLibWrapper {
public:
  void Unpack(const char* dat, size_t dat_size,
      std::vector<char>& unpack) const {
    z_stream z;
    memset(&z, 0, sizeof(z));

    z.next_in = (unsigned char*) dat;
    z.avail_in = dat_size;
    z.next_out = (unsigned char*) unpack.data();
    z.avail_out = unpack.size();

    if (inflateInit(&z) != Z_OK)
      throw Exception("%s", "Can't init zlib");

    if (inflate(&z, Z_FINISH) != Z_STREAM_END)
      throw Exception("%s", "Can't inflate zlib");

    if (inflateEnd(&z) != Z_OK)
      throw Exception("%s", "Can't deinit zlib");

    unpack.resize(z.total_out);
  }
};

} // namespace geo_base

#endif // GEO_BASE_ZLIB_WRAPPER_H_
