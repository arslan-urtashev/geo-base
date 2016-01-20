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

#include <geo_base/open_street_map/parser.h>
#include <geo_base/util/log.h>
#include <geo_base/wrappers/std.h>

namespace geo_base {
namespace open_street_map {

class weak_consistency_checker_t : public parser_t {
public:
    weak_consistency_checker_t(allocator_t *allocator)
        : parser_t(allocator)
    {
    }

    void process_node(geo_id_t geo_id, location_t const &, kvs_t const &) override
    {
        nodes_.insert(geo_id);
    }

    void process_way(geo_id_t geo_id, kvs_t const &, geo_ids_t const &nodes) override
    {
        for (geo_id_t const &id : nodes)
            expect_nodes_.insert(id);
        ways_.insert(geo_id);
    }

    void process_relation(geo_id_t, kvs_t const &, references_t const &refs) override
    {
        for (reference_t const &r: refs)
            if (r.type == reference_t::TYPE_WAY)
                expect_ways_.insert(r.geo_id);
    }

    size_t check_nodes() const
    {
        size_t not_found = 0;
        for (geo_id_t const &id : expect_nodes_)
            if (nodes_.find(id) == nodes_.end())
                ++not_found;
        return not_found;
    }

    size_t check_ways() const
    {
        size_t not_found = 0;
        for (geo_id_t const &id : expect_ways_)
            if (ways_.find(id) == ways_.end())
                ++not_found;
        return not_found;
    }

    void merge(weak_consistency_checker_t const &c)
    {
        nodes_.insert(c.nodes_.begin(), c.nodes_.end());
        expect_nodes_.insert(c.expect_nodes_.begin(), c.expect_nodes_.end());
        ways_.insert(c.ways_.begin(), c.ways_.end());
        expect_ways_.insert(c.expect_ways_.begin(), c.expect_ways_.end());
    }

private:
    set_t<geo_id_t> nodes_;
    set_t<geo_id_t> expect_nodes_;
    set_t<geo_id_t> ways_;
    set_t<geo_id_t> expect_ways_;
};

} // namespace open_street_map
} // namespace geo_base
