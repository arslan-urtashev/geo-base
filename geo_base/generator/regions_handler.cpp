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

#include <geo_base/core/kv.h>
#include <geo_base/core/polygon.h>
#include <geo_base/core/region.h>
#include <geo_base/library/log.h>
#include <geo_base/generator/regions_handler.h>

namespace geo_base {
namespace generator {

void regions_handler_t::update(proto::region_t const &proto_region)
{
    region_t region;
    memset(&region, 0, sizeof(region));

    region.region_id = proto_region.region_id();
    region.kvs_offset = geo_data_->kvs_number();

    for (proto::kv_t const &kv : proto_region.kvs()) {
        kv_t x;
        x.k = geo_data_->insert(kv.k());
        x.v = geo_data_->insert(kv.v());
        geo_data_->kvs_append(x);
    }

    region.kvs_number = geo_data_->kvs_number() - region.kvs_offset;
    geo_data_->regions_append(region);
}

void regions_handler_t::fini()
{
    region_t *regions = geo_data_->mut_regions();
    region_t *regions_end = geo_data_->regions_number() + regions;

    std::stable_sort(regions, regions_end);

    for (size_t i = 0; i < geo_data_->polygons_number(); ++i) {
        polygon_t const &p = geo_data_->polygons()[i];
        region_t *r = std::lower_bound(regions, regions_end, p.region_id);
        if (r == regions_end || r->region_id != p.region_id) {
            log_warning("Region %lu not exists!", p.region_id);
            continue;
        }
        r->square += p.square;
        r->polygons_number++;
    }
}

} // namespace generator
} // namespace geo_base
