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

#include <geo_base/core/geo_data/map.h>
#include <geo_base/generator/generator.h>
#include <geo_base/generator/gen_geo_data.h>
#include <geo_base/generator/mut_geo_data.h>
#include <geo_base/generator/slab_handler.h>
#include <geo_base/generator/raw_borders_handler.h>
#include <geo_base/generator/regions_handler.h>
#include <geo_base/library/log.h>
#include <geo_base/library/pool_allocator.h>
#include <geo_base/library/base_allocator.h>
#include <geo_base/proto_library/reader.h>

namespace geo_base {
namespace generator {

generator_t::generator_t(config_t const &config, gen_geo_data_t *geo_data, allocator_t *allocator)
    : geo_data_(geo_data)
    , allocator_(allocator)
    , handlers_()
    , config_(config)
{
#define HANDLER(handler_t) \
    handler_ptr_t(new handler_t(config_, geo_data_, allocator_))

    handlers_ = handler_ptrs_t{
        HANDLER(slab_handler_t),
        HANDLER(raw_borders_handler_t),
        HANDLER(regions_handler_t),
    };

#undef HANDLER
}

void generator_t::init()
{
    for (handler_ptr_t &h : handlers_)
        h->init();
}

void generator_t::update(proto::region_t const &region)
{
    for (handler_ptr_t &h : handlers_)
        h->update(region);
}

void generator_t::fini()
{
    for (handler_ptr_t &h : handlers_)
        h->fini();
}

void generate(char const *in, char const *out)
{
    pool_allocator_t allocator(128_mb);

    mut_geo_data_t geo_data;
    generator_t generator(config_t(), &geo_data, &allocator);

    generator.init();

    proto::reader_t reader(in);
    reader.each([&] (proto::region_t const &region) {
        generator.update(region);
    });

    generator.fini();

    base_allocator_t base_allocator(out);
    geo_data_map_t map(geo_data, &base_allocator);
}

} // namespace generator
} // namespace geo_base
