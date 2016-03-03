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
#include <geo_base/proto/def/region.pb.h>
#include <geo_base/library/dynarray.h>
#include <geo_base/library/file.h>
#include <geo_base/library/file_stream.h>
#include <geo_base/library/safe_stream.h>

namespace geo_base {
namespace proto {

class writer_t {
public:
    explicit writer_t(char const *path)
        : file_()
        , output_stream_()
        , safe_output_stream_()
    {
        file_.read_write_open(path);
        output_stream_ = file_output_stream_t(file_.fd());
        safe_output_stream_ = safe_output_stream_t(&output_stream_);
    }

    template<typename message_t>
    void write(message_t const &message, allocator_t *allocator)
    {
        uint32_t const byte_size = message.ByteSize();
        uint32_t const total_size = byte_size + sizeof(byte_size);

        dynarray_t<char> buffer(total_size, total_size, allocator);

        *((uint32_t *) buffer.data()) = htonl(byte_size);

        if (!message.SerializeToArray(buffer.data() + sizeof(byte_size), byte_size))
            throw exception_t("Unable to serialize message to array");

        if (!safe_output_stream_.write(buffer.data(), buffer.size()))
            throw exception_t("Unable writer serialized message");
    }

private:
    file_t file_;
    file_output_stream_t output_stream_;
    safe_output_stream_t safe_output_stream_;

    GEO_BASE_DISALLOW_EVIL_CONSTRUCTORS(writer_t);
};

} // namespace proto
} // namespace geo_base
