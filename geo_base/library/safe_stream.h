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

#include <geo_base/library/io_stream.h>
#include <mutex>
#include <algorithm>

namespace geo_base {

class safe_output_stream_t : public output_stream_t {
public:
    safe_output_stream_t()
        : mutex_()
        , output_stream_(nullptr)
    { }

    explicit safe_output_stream_t(output_stream_t *output_stream)
        : mutex_()
        , output_stream_(output_stream)
    { }

    safe_output_stream_t(safe_output_stream_t &&s)
        : mutex_()
        , output_stream_(nullptr)
    {
        std::lock_guard<std::mutex> lock1(mutex_);
        std::lock_guard<std::mutex> lock2(s.mutex_);
        std::swap(output_stream_, s.output_stream_);
    }

    safe_output_stream_t &operator = (safe_output_stream_t &&s)
    {
        std::lock_guard<std::mutex> lock1(mutex_);
        std::lock_guard<std::mutex> lock2(s.mutex_);
        std::swap(output_stream_, s.output_stream_);
        return *this;
    }

    bool write(char const *ptr, size_t count) override
    {
        std::lock_guard<std::mutex> lock(mutex_);
        return output_stream_->write(ptr, count);
    }

private:
    std::mutex mutex_;
    output_stream_t *output_stream_;

    GEO_BASE_DISALLOW_EVIL_CONSTRUCTORS(safe_output_stream_t);
};

} // namespace geo_base
