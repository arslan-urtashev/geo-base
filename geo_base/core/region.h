// Copyright (c) 2015, 2016 Urtashev Arslan. All rights reserved.
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

#include <geo_base/core/common.h>

namespace geo_base {

struct region_t {
    using options_t = uint64_t;

    enum option_t : options_t {
        OPTION_BOUNDARY_ADMINISTRATIVE = 1u << 0,
        OPTION_BOUNDARY_HISTORIC       = 1u << 1,
        OPTION_BOUNDARY_MARITIME       = 1u << 2,
        OPTION_BOUNDARY_NATIONAL_PARK  = 1u << 3,
        OPTION_BOUNDARY_POLITICAL      = 1u << 4,
        OPTION_BOUNDARY_PROTECTED_AREA = 1u << 5,
        OPTION_PLACE_ALLOTMENTS        = 1u << 6,
        OPTION_PLACE_BOROUGH           = 1u << 7,
        OPTION_PLACE_CITY              = 1u << 8,
        OPTION_PLACE_CITY_BLOCK        = 1u << 9,
        OPTION_PLACE_CONTINENT         = 1u << 10,
        OPTION_PLACE_COUNTRY           = 1u << 11,
        OPTION_PLACE_DISTRICT          = 1u << 12,
        OPTION_PLACE_FARM              = 1u << 13,
        OPTION_PLACE_HAMLET            = 1u << 14,
        OPTION_PLACE_ISLAND            = 1u << 15,
        OPTION_PLACE_LOCALITY          = 1u << 16,
        OPTION_PLACE_MUNICIPALITY      = 1u << 17,
        OPTION_PLACE_NEIGHBOURHOOD     = 1u << 18,
        OPTION_PLACE_PLOT              = 1u << 19,
        OPTION_PLACE_PROVINCE          = 1u << 20,
        OPTION_PLACE_QUARTER           = 1u << 21,
        OPTION_PLACE_REGION            = 1u << 22,
        OPTION_PLACE_STATE             = 1u << 23,
        OPTION_PLACE_SUBURB            = 1u << 24,
        OPTION_PLACE_TOWN              = 1u << 25,
        OPTION_PLACE_VILLAGE           = 1u << 26,
    };

    geo_id_t region_id;
    number_t kvs_offset;
    number_t kvs_number;
    square_t square;
    options_t options;
    number_t polygons_number;

    bool operator == (region_t const &r) const
    {
        return region_id == r.region_id;
    }

    bool operator < (region_t const &r) const
    {
        return region_id < r.region_id;
    }

    bool operator < (geo_id_t const &r) const
    {
        return region_id < r;
    }

    friend bool operator < (geo_id_t const &region_id, region_t const &r)
    {
        return region_id < r.region_id;
    }

    bool better(region_t const &r) const
    {
        return square < r.square;
    }
};

} // namespace geo_base
