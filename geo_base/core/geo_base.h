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

#include <vector>
#include <functional>

#include <geo_base/core/common.h>
#include <geo_base/core/geo_data/geo_data.h>
#include <geo_base/core/geo_data/proxy.h>
#include <geo_base/lib/mem_file.h>

namespace geo_base {

geo_id_t const UNKNOWN_GEO_ID = static_cast<geo_id_t>(-1);

class geo_base_t {
public:
    using debug_t = std::vector<geo_id_t>;
    using kv_callback_t = std::function<void (char const *, char const *)>;
    using polygon_callback_t = std::function<void (polygon_t const &)>;
    using part_callback_t = std::function<void (part_t const &, number_t)>;

    geo_base_t()
        : geo_data_proxy_()
    { }

    geo_base_t(geo_base_t &&g)
        : geo_data_proxy_()
    {
        std::swap(geo_data_proxy_, g.geo_data_proxy_);
    }

    geo_base_t &operator = (geo_base_t &&g)
    {
        std::swap(geo_data_proxy_, g.geo_data_proxy_);
        return *this;
    }

    explicit geo_base_t(char const *path)
        : geo_data_proxy_(new geo_data_map_proxy_t(path))
    { }

    explicit geo_base_t(geo_data_t const &geo_data)
        : geo_data_proxy_(new geo_data_wrapper_t(geo_data))
    { }

    geo_id_t lookup(location_t const &location, debug_t *debug = nullptr) const;

    bool each_kv(geo_id_t region_id, kv_callback_t callback) const;

    void each_polygon(polygon_callback_t callback) const;

    void each_part(polygon_t const &polygon, part_callback_t callback) const;

    geo_data_t const &geo_data() const
    {
        return *geo_data_proxy_->geo_data();
    }

private:
    geo_data_proxy_ptr_t geo_data_proxy_;

    GEO_BASE_DISALLOW_EVIL_CONSTRUCTORS(geo_base_t);
};

}
