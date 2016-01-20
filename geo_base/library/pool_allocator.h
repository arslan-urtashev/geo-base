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

#include <geo_base/library/block_allocator.h>
#include <geo_base/library/mem_guard.h>

namespace geo_base {

class pool_allocator_t : public block_allocator_t {
public:
    pool_allocator_t()
        : mem_guard_()
    { }

    pool_allocator_t(pool_allocator_t &&a)
        : block_allocator_t(std::forward<block_allocator_t>(a))
        , mem_guard_()
    {
        std::swap(mem_guard_, a.mem_guard_);
    }

    pool_allocator_t &operator = (pool_allocator_t &&a)
    {
        block_allocator_t::operator = (std::forward<block_allocator_t>(a));
        std::swap(mem_guard_, a.mem_guard_);
        return *this;
    }

    explicit pool_allocator_t(size_t pool_size);

private:
    mem_guard_t mem_guard_;

    GEO_BASE_DISALLOW_EVIL_CONSTRUCTORS(pool_allocator_t);
};

} // namespace geo_base
