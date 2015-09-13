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

#ifndef GEO_BASE_ALGO_H_
#define GEO_BASE_ALGO_H_

#include <algorithm>

namespace geo_base {

struct Less {
  template<typename T, typename U>
  bool operator () (const T& a, const U& u) const {
    return a < u; 
  }
};

template<typename T, typename U, typename Cmp = Less>
T LowerBound(T a, size_t count, const U& x, Cmp cmp = Cmp()) {
  return std::lower_bound(a, a + count, x, cmp);
}

template<typename T, typename U, typename Cmp = Less>
T Find(T a, size_t count, const U& x, Cmp cmp = Cmp()) {
  T ret = LowerBound(a, count, x, cmp);
  return (ret >= a && ret < a + count && !cmp(*ret, x) && !cmp(x, *ret)) ? ret : NULL;
}

template<typename T>
const T& Max(const T& x, const T& y) {
  return std::max(x, y);
}

template<typename T>
const T& Min(const T& x, const T& y) {
  return std::min(x, y);
}

} // namespace geo_base

#endif // GEO_BASE_ALGO_H_
