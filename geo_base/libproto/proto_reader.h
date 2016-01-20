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

#include <arpa/inet.h>
#include <geo_base/core/common.h>
#include <geo_base/proto/region.pb.h>
#include <geo_base/lib/allocator.h>
#include <geo_base/lib/mem_file.h>
#include <geo_base/wrappers/std.h>

namespace geo_base {

class proto_reader_t {
public:
    proto_reader_t(char const *path)
        : mem_file_()
        , index_()
    {
        mem_file_.read_open(path);
    }

    template<typename callback_t>
    void each(callback_t callback)
    {
        each_with_ptr([&] (char const *, proto::region_t const &region) {
            callback(region);
        });
    }

    template<typename callback_t>
    bool region(geo_id_t geo_id, callback_t callback)
    {
        if (index_.empty()) {
            each([&] (proto::region_t const &region) {
                if (region.region_id() == geo_id)
                    callback(region);
            });

        } else {
            if (index_.find(geo_id)    == index_.end())
                return false;

            char const *ptr = index_[geo_id];
            uint32_t const byte_size = ntohl(*((uint32_t *) ptr));

            proto::region_t region;
            if (!region.ParseFromArray(ptr + sizeof(byte_size), byte_size))
                throw exception_t("Unable parse region from array");

            callback(region);
        }

        return true;
    }

    void generate_index();

private:
    template<typename callback_t>
    void each_with_ptr(callback_t callback)
    {
        char const *begin = (char const *) mem_file_.data();
        char const *end = begin + mem_file_.size();

        char const *ptr = begin;
        proto::region_t region;

        while (ptr < end) {
            uint32_t const byte_size = ntohl(*((uint32_t *) ptr));
            if (!region.ParseFromArray(ptr + sizeof(byte_size), byte_size))
                throw exception_t("Unable parse region from array");
            callback(ptr, (proto::region_t const &) region);
            ptr += byte_size + sizeof(byte_size);
        }
    }

    mem_file_t mem_file_;
    map_t<geo_id_t, char const *> index_;
};

} // namespace geo_base
