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

#pragma once

#include <algorithm>

namespace geo_base {

struct less_t {
    template<typename a_t, typename b_t>
    bool operator () (a_t const &a, b_t const &b) const
    {
        return a < b;
    }
};

template<typename arr_t, typename val_t, typename cmp_t = less_t>
arr_t lower_bound(arr_t arr, size_t count, val_t const &val, cmp_t cmp = cmp_t())
{
    return std::lower_bound(arr, arr + count, val, cmp);
}

template<typename arr_t, typename val_t, typename cmp_t = less_t>
arr_t find(arr_t arr, size_t count, val_t const &val, cmp_t cmp = cmp_t())
{
    arr_t ret = lower_bound(arr, count, val, cmp);
    return !cmp(*ret, val) && !cmp(val, *ret) ? ret : nullptr;
}

} // namespace geo_base
