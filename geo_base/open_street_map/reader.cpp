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

#include <arpa/inet.h>

#include <geo_base/open_street_map/reader.h>
#include <geo_base/library/dynarray.h>
#include <geo_base/library/log.h>

namespace geo_base {
namespace open_street_map {

bool reader_t::read(proto::blob_header_t *header, proto::blob_t *blob, allocator_t *allocator)
{
    std::lock_guard<std::mutex> const lock(mutex_);

    int32_t header_size = 0;
    if (!input_stream_->read((char *) &header_size, sizeof(header_size)))
        return false;

    header_size = ntohl(header_size);

    dynarray_t<char> raw_header(header_size, header_size, allocator);
    if (!input_stream_->read(raw_header.data(), raw_header.size())) {
        log_error("Unable read header!");
        return false;
    }

    if (!header->ParseFromArray(raw_header.data(), raw_header.size())) {
        log_error("Unable parse header!");
        return false;
    }

    int32_t const blob_size = header->data_size();

    dynarray_t<char> raw_blob(blob_size, blob_size, allocator);
    if (!input_stream_->read(raw_blob.data(), raw_blob.size())) {
        log_error("Unable read blob!");
        return false;
    }

    if (!blob->ParseFromArray(raw_blob.data(), raw_blob.size())) {
        log_error("Unable parse blob!");
        return false;
    }

    return true;
}

} // namespace open_street_map
} // namespace geo_base
