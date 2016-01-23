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

#include <thread>
#include <future>

#include <geo_base/core/location.h>
#include <geo_base/open_street_map/open_street_map.h>
#include <geo_base/open_street_map/proto/open_street_map.pb.h>
#include <geo_base/open_street_map/reader.h>
#include <geo_base/lib/allocator.h>
#include <geo_base/lib/dynarray.h>
#include <geo_base/lib/file.h>
#include <geo_base/lib/file_stream.h>
#include <geo_base/lib/log.h>
#include <geo_base/lib/stop_watch.h>

namespace geo_base {
namespace open_street_map {

class parser_t {
public:
    parser_t()
        : allocator_(nullptr)
        , blocks_processed_(0)
        , nodes_processed_(0)
        , dense_nodes_processed_(0)
        , relations_processed_(0)
        , ways_processed_(0)
        , processing_disabled_mask_(0)
    { }

    explicit parser_t(allocator_t *allocator)
        : allocator_(allocator)
        , blocks_processed_(0)
        , nodes_processed_(0)
        , dense_nodes_processed_(0)
        , relations_processed_(0)
        , ways_processed_(0)
        , processing_disabled_mask_(0)
    { }

    parser_t(parser_t &&p)
        : parser_t()
    {
        std::swap(allocator_, p.allocator_);
        std::swap(blocks_processed_, p.blocks_processed_);
        std::swap(nodes_processed_, p.nodes_processed_);
        std::swap(dense_nodes_processed_, p.dense_nodes_processed_);
        std::swap(relations_processed_, p.relations_processed_);
        std::swap(ways_processed_, ways_processed_);
        std::swap(processing_disabled_mask_, processing_disabled_mask_);
    }

    void parse(reader_t *reader);

    size_t blocks_processed() const
    {
        return blocks_processed_;
    }

    size_t nodes_processed() const
    {
        return nodes_processed_;
    }

    size_t dense_nodes_processed() const
    {
        return dense_nodes_processed_;
    }

    size_t relations_processed() const
    {
        return relations_processed_;
    }

    size_t ways_processed() const
    {
        return ways_processed_;
    }

protected:
    virtual void process_node(geo_id_t, location_t const &, kvs_t const &)
    {
        processing_disabled_mask_ |= NODE_PROC_DISABLED;
    }

    virtual void process_way(geo_id_t, kvs_t const &, geo_ids_t const &)
    {
        processing_disabled_mask_ |= WAY_PROC_DISABLED;
    }

    virtual void process_relation(geo_id_t, kvs_t const &, references_t const &)
    {
        processing_disabled_mask_ |= RELATION_PROC_DISABLED;
    }

    allocator_t *allocator_;

private:
    enum processing_disabled_t {
        NODE_PROC_DISABLED     = (1u << 0),
        WAY_PROC_DISABLED      = (1u << 1),
        RELATION_PROC_DISABLED = (1u << 2),
    };

    void process_basic_groups(proto::basic_block_t const &block);

    void process_dense_nodes(proto::dense_nodes_t const &nodes, proto::basic_block_t const &block);

    size_t blocks_processed_;
    size_t nodes_processed_;
    size_t dense_nodes_processed_;
    size_t relations_processed_;
    size_t ways_processed_;
    uint64_t processing_disabled_mask_;

    GEO_BASE_DISALLOW_EVIL_CONSTRUCTORS(parser_t);
};

class pool_parser_t {
public:
    pool_parser_t()
    { }

    template<typename container_t>
    void parse(reader_t *reader, container_t &container)
    {
        std::vector<std::thread> threads(container.size());
        for (size_t i = 0; i < threads.size(); ++i) {
            threads[i] = std::thread([&container, &reader, i] () {
                container[i].parse(reader);
            });
        }
        for (size_t i = 0; i < threads.size(); ++i)
            threads[i].join();
    }

    template<typename container_t>
    void parse(char const *path, container_t &container)
    {
        char resolved_path[PATH_MAX];
        log_info("Run parser on %s", realpath(path, resolved_path));

        stop_watch_t stop_watch;
        stop_watch.run();

        {
            file_t file;
            file.read_open(path);
            file_input_stream_t input_stream(file.fd());
            reader_t reader(&input_stream);
            parse(&reader, container);
        }

        float const seconds = stop_watch.get();
        log_info("Parsed %s in %.3f seconds (%.3f minutes)",
            realpath(path, resolved_path), seconds, seconds / 60.0);
    }

private:
    GEO_BASE_DISALLOW_EVIL_CONSTRUCTORS(pool_parser_t);
};

size_t optimal_threads_number();

} // namespace open_street_map
} // namespace geo_base
