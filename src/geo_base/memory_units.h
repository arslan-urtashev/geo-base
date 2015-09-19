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

#ifndef GEO_BASE_MEMORY_UNITS_H_
#define GEO_BASE_MEMORY_UNITS_H_

#include "io.h"

namespace geo_base {

struct Bytes {
  size_t bytes_count;

  explicit Bytes(size_t count) :
      bytes_count(count) {
  }
};

// Fake structure for convert bytes to megabytes in output.
struct Megabytes : public Bytes {
  explicit Megabytes(size_t count) :
      Bytes(count) {
  }
};

inline OutputStream& operator << (OutputStream& out, const Megabytes& m) {
  out << m.bytes_count / (1024. * 1024.) << " Mb";
  return out;
}

// Fake structure for convert bytes to gigabytes in output.
struct Gigabytes : public Bytes {
  explicit Gigabytes(size_t count) :
      Bytes(count) {
  }
};

inline OutputStream& operator << (OutputStream& out, const Gigabytes& g) {
  static const size_t kOneGigabyte = 1024 * 1024 * 1024;

  if (g.bytes_count >= kOneGigabyte)
    out << g.bytes_count * 1.0 / kOneGigabyte  << " Gb";
  else
    out << Megabytes(g.bytes_count);
  return out;
}

inline OutputStream& operator << (OutputStream& out, const Bytes& b) {
  out << Megabytes(b.bytes_count);
  return out;
}

} // namespace geo_base

#endif // GEO_BASE_MEMORY_UNITS_H_
