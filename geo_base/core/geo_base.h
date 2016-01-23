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
#include <geo_base/core/geo_data/geo_data.h>
#include <geo_base/core/geo_data/loader.h>
#include <geo_base/lib/mem_file.h>

namespace geo_base {

geo_id_t const UNKNOWN_GEO_ID = static_cast<geo_id_t>(-1);

class geo_base_t {
public:
    geo_base_t()
        : geo_data_loader_()
    { }

    geo_base_t(geo_base_t &&g)
        : geo_data_loader_()
    {
        std::swap(geo_data_loader_, g.geo_data_loader_);
    }

    geo_base_t &operator = (geo_base_t &&g)
    {
        std::swap(geo_data_loader_, g.geo_data_loader_);
        return *this;
    }

    explicit geo_base_t(char const *path)
        : geo_data_loader_(new geo_data_map_loader_t(path))
    { }

    explicit geo_base_t(geo_data_t const &geo_data)
        : geo_data_loader_(new geo_data_wrapper_t(geo_data))
    { }

    geo_id_t lookup(location_t const &location) const;

    template<typename callback_t>
    bool kv(geo_id_t region_id, callback_t callback) const;

private:
    geo_data_loader_ptr_t geo_data_loader_;

    GEO_BASE_DISALLOW_EVIL_CONSTRUCTORS(geo_base_t);
};

template<typename callback_t>
bool geo_base_t::kv(geo_id_t region_id, callback_t callback) const
{
    geo_data_t const &g = *geo_data_loader_->geo_data();

    region_t const *begin = g.regions();
    region_t const *end = begin + g.regions_number();

    region_t const *region = std::lower_bound(begin, end, region_id);

    if (region == end || region->region_id != region_id)
        return false;

    kv_t const *kvs = g.kvs() + region->kvs_offset;
    char const *blobs = g.blobs();

    for (number_t i = 0; i < region->kvs_number; ++i) {
        char const *k = blobs + kvs[i].k;
        char const *v = blobs + kvs[i].v;
        callback(k, v);
    }
}

}
