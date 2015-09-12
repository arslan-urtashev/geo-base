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

#ifndef GEO_BASE_HASH_H_
#define GEO_BASE_HASH_H_

#include "algo.h"

namespace geo_base {

struct Hash64 {
  std::hash<uint64_t> hash;

  template<typename T>
  uint64_t operator () (const T& x) const {
    static_assert(sizeof(x) == sizeof(uint64_t), "sizeof(x) != sizeof(uint64_t)");
    return hash(*((const uint64_t*) &x));
  }
};

// Simple polynomial hash with custom base (kBase).
// Polynomial hash is a polynom x[0] * p^0 + x[1] * p^1 + x[2] * p^2 etc.
template<const uint64_t kBase = 337>
struct PolynomialHash {
  uint64_t operator () (const char *bytes, const Count count) const {
    uint64_t hash = 0, power = 1;
    for (Count i = 0; i < count; ++i) {
      hash += (bytes[i] + 1) * power;
      power *= kBase;
    }
    return hash;
  }

  template<typename T>
  uint64_t operator () (const T& val) const {
    return (*this)((const char*) &val, sizeof(T));
  }

  template<typename T>
  uint64_t operator () (const std::vector<T>& a) const {
    return (*this)((const char*) &(a[0]), sizeof(T) * a.size());
  }
};

} // namespace geo_base

#endif // GEO_BASE_HASH_H_
