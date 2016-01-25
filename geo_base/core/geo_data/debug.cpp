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

#include <vector>

#include <geo_base/core/geo_data/debug.h>
#include <geo_base/lib/log.h>
#include <geo_base/lib/exception.h>

namespace geo_base {
namespace geo_data {

size_t space(geo_data_t const &g)
{
    size_t space = 0;

#define GEO_BASE_DEF_VAR(var_t, var) \
    space += sizeof(var_t);

#define GEO_BASE_DEF_ARR(arr_t, arr) \
    space += sizeof(number_t) + sizeof(arr_t) * g.arr##_number();

    GEO_BASE_DEF_GEO_DATA

#undef GEO_BASE_DEF_VAR
#undef GEO_BASE_DEF_ARR

    return space;
}

template<typename arr_t>
static float array_space(number_t number)
{
    return number * sizeof(arr_t) / (1024.0 * 1024.0);
}

void show(int fd, geo_data_t const &g)
{
    dprintf(fd, "geo_data = %.3f Gb\n", geo_data::space(g) / (1024.0 * 1024.0 * 1024.0));

#define GEO_BASE_DEF_VAR(var_t, var) \
    dprintf(fd, "  geo_data.%s = %llu\n", #var, (unsigned long long) g.var());

#define GEO_BASE_DEF_ARR(arr_t, arr) \
    dprintf(fd, "  geo_data.%s = %u x %lu = %.3f Mb\n", \
        #arr, g.arr##_number(), sizeof(arr_t), array_space<arr_t>(g.arr##_number()));

    GEO_BASE_DEF_GEO_DATA

#undef GEO_BASE_DEF_VAR
#undef GEO_BASE_DEF_ARR
}

template<typename arr_t>
static bool equals(arr_t const *a, arr_t const *b, size_t count)
{
    return !memcmp(a, b, sizeof(arr_t) * count);
}

bool equals(geo_data_t const &a, geo_data_t const &b)
{
#define GEO_BASE_DEF_VAR(var_t, var) \
    if (a.var() != b.var()) { \
        log_error(#var " not equal"); \
        return false; \
    }

#define GEO_BASE_DEF_ARR(arr_t, arr) \
    GEO_BASE_DEF_VAR(number_t, arr##_number); \
    if (!equals(a.arr(), b.arr(), a.arr##_number())) { \
        log_error(#arr " not equal"); \
        return false; \
    }

    GEO_BASE_DEF_GEO_DATA

#undef GEO_BASE_DEF_VAR
#undef GEO_BASE_DEF_ARR

    return true;
}

} // namespace geo_data
} // namespace geo_base
