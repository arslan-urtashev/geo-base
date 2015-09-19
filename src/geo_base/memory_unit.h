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

#ifndef GEO_BASE_MEMORY_UNIT_H_
#define GEO_BASE_MEMORY_UNIT_H_

#include "io.h"

#include <iomanip>

namespace geo_base {

// Structure for user friendly memory size output.
struct MemoryUnit {
  size_t bytes_count;

  explicit MemoryUnit(size_t count) :
      bytes_count(count) {
  }
};

inline OutputStream& operator << (OutputStream& out, const MemoryUnit& b) {
  static const size_t kOneKb = 1024;
  static const size_t kOneMb = kOneKb * 1024;
  static const size_t kOneGb = kOneMb * 1024;

  out << std::fixed << std::setprecision(2);

  if (b.bytes_count < kOneKb)
    out << b.bytes_count << " B";
  else if (b.bytes_count < kOneMb)
    out << b.bytes_count / 1024. << " Kb";
  else if (b.bytes_count < kOneGb)
    out << b.bytes_count / (1024. * 1024.) << " Mb";
  else
    out << b.bytes_count / (1024. * 1024. * 1024.) << " Gb";

  return out;
}

} // namespace geo_base

#endif // GEO_BASE_MEMORY_UNIT_H_
