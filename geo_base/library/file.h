// Copyright (c) 2015 Urtashev Arslan. All rights reserved.
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

#include <geo_base/library/fd_guard.h>
#include <geo_base/library/common.h>

namespace geo_base {

class file_t {
public:
    file_t()
        : fd_guard_()
    { }

    file_t(file_t &&f)
        : fd_guard_()
    {
        std::swap(fd_guard_, f.fd_guard_);
    }

    file_t &operator = (file_t &&f)
    {
        std::swap(fd_guard_, f.fd_guard_);
        return *this;
    }

    void read_open(char const *path);

    void read_write_open(char const *path);

    int fd() const
    {
        return fd_guard_.fd();
    }

private:
    fd_guard_t fd_guard_;

    GEO_BASE_DISALLOW_EVIL_CONSTRUCTORS(file_t);
};

} // namespace geo_base
