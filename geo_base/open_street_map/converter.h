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
#include <geo_base/proto/def/region.pb.h>
#include <geo_base/libpb/proto_writer.h>
#include <geo_base/wrappers/std.h>

namespace geo_base {
namespace open_street_map {

typedef set_t<geo_id_t> geo_ids_set_t;
typedef map_t<geo_id_t, vector_t<geo_id_t>> ways_map_t;
typedef map_t<geo_id_t, location_t> nodes_map_t;

class grep_boundary_ways_t : public parser_t {
public:
    grep_boundary_ways_t(allocator_t *allocator)
        : parser_t(allocator)
    { }

    void process_way(geo_id_t, kvs_t const &, geo_ids_t const &) override;

    void process_relation(geo_id_t, kvs_t const &, references_t const &) override;

    geo_ids_set_t const &ways() const
    {
        return ways_;
    }

    void clear()
    {
        ways_.clear();
    }

private:
    geo_ids_set_t ways_;
};

class grep_boundary_node_ids_t : public parser_t {
public:
    grep_boundary_node_ids_t(geo_ids_set_t const &ways, allocator_t *allocator)
        : parser_t(allocator)
        , need_ways_(&ways)
    { }

    void process_way(geo_id_t geo_id, kvs_t const &, geo_ids_t const &node_ids) override;

    void clear()
    {
        ways_.clear();
        nodes_.clear();
    }

    geo_ids_set_t const &nodes() const
    {
        return nodes_;
    }

    ways_map_t const &ways() const
    {
        return ways_;
    }

private:
    ways_map_t ways_;
    geo_ids_set_t const *need_ways_;
    geo_ids_set_t nodes_;
};

class grep_boundary_nodes_t : public parser_t {
public:
    grep_boundary_nodes_t(geo_ids_set_t const &nodes, allocator_t *allocator)
        : parser_t(allocator)
        , need_nodes_(&nodes)
    { }

    void process_node(geo_id_t geo_id, location_t const &location, kvs_t const &) override
    {
        if (need_nodes_->find(geo_id) != need_nodes_->end())
            nodes_[geo_id] = location;
    }

    void clear()
    {
        nodes_.clear();
    }

    nodes_map_t const &nodes() const
    {
        return nodes_;
    }

private:
    geo_ids_set_t const *need_nodes_;
    nodes_map_t nodes_;
};

class converter_t : public parser_t {
public:
    converter_t(nodes_map_t const &nodes, ways_map_t const &ways, proto_writer_t *writer,
                allocator_t *allocator)
        : parser_t(allocator)
        , writer_(writer)
        , ways_(&ways)
        , nodes_(&nodes)
        , regions_number_(0)
    { }

    void process_way(geo_id_t geo_id, kvs_t const &kvs, geo_ids_t const &nodes) override;

    void process_relation(geo_id_t geo_id, kvs_t const &kvs, references_t const &refs) override;

    size_t regions_number() const
    {
        return regions_number_;
    }

private:
    proto_writer_t *writer_;
    ways_map_t const *ways_;
    nodes_map_t const *nodes_;
    size_t regions_number_;
};

// Run all convertion in different threads. Read open_street_map data from input_path file
// and write geo_base protobuf into output_path.
void run_pool_convert(char const *input_path, char const *output_path, size_t threads_number);

} // namespace open_street_map
} // namespace geo_base
