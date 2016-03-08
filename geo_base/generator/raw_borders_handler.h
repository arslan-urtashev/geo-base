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

#include <geo_base/generator/handler.h>
#include <geo_base/generator/gen_geo_data.h>
#include <geo_base/library/dynarray.h>
#include <geo_base/library/stop_watch.h>

namespace geo_base {
namespace generator {

class raw_borders_handler_t : public handler_t {
public:
    raw_borders_handler_t(config_t const &config, gen_geo_data_t *geo_data, allocator_t *allocator)
        : handler_t(config, geo_data, allocator)
    { }

    void init() override;

    void update(proto::region_t const &region) override;

    void fini() override;

private:
    void update(geo_id_t region_id, geo_id_t polygon_id, dynarray_t<point_t> const &raw_points,
        raw_border_t::type_t type);

    void update(geo_id_t region_id, proto::polygon_t const &polygon);
};

} // namespace generator
} // namespace geo_base
