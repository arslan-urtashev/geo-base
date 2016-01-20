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

#include <geo_base/core/common.h>
#include <geo_base/open_street_map/proto/open_street_map.pb.h>
#include <geo_base/util/dynarray.h>

namespace geo_base {
namespace open_street_map {

struct kv_t {
    char const *k;
    char const *v;
};

struct reference_t {
    enum type_t {
        TYPE_NODE     = proto::relation_t::NODE,
        TYPE_WAY      = proto::relation_t::WAY,
        TYPE_RELATION = proto::relation_t::RELATION,
    };

    geo_id_t geo_id;
    type_t type;
    char const *role;
};

typedef dynarray_t<kv_t> kvs_t;
typedef dynarray_t<reference_t> references_t;
typedef dynarray_t<geo_id_t> geo_ids_t;

char const *find_name(kvs_t const &kvs);

} // namespace open_street_map
} // namespace geo_base
