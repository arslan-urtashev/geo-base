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

#pragma once

#include <geo_base/core/geo_data/geo_data.h>

namespace geo_base {
namespace generator {

class gen_geo_data_t : public geo_data_t {
#define GEO_BASE_DEF_VAR(var_t, var) \
    virtual void set_##var(var_t const &var) = 0;

#define GEO_BASE_DEF_ARR(arr_t, arr) \
    virtual arr_t *mut_##arr() = 0; \
    virtual void arr##_append(arr_t const &arr) = 0;

public:
    GEO_BASE_DEF_GEO_DATA

#undef GEO_BASE_DEF_VAR
#undef GEO_BASE_DEF_ARR

    // Insert unique point into points array.
    virtual ref_t insert(point_t const &p) = 0;

    // Insert unique edge into edges array.
    virtual ref_t insert(edge_t const &e) = 0;

    // Inser unique blob into blobs array.
    virtual ref_t insert(std::string const &s) = 0;
};

} // namespace generator
} // namespace geo_base
