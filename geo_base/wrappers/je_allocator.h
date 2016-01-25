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

#include <geo_base/lib/allocator.h>
#include <jemalloc/jemalloc.h>

namespace geo_base {

template<typename value_t>
class je_allocator_t {
public:
    using value_type = value_t;
    using pointer = value_t*;
    using reference = value_t&;
    using const_pointer = value_t*;
    using const_reference = value_t const &;
    using size_type = size_t;
    using difference_type = ptrdiff_t;

    je_allocator_t()
    { }

    template<typename other_t>
    je_allocator_t(je_allocator_t<other_t> const &)
    { }

    template<typename other_t>
    struct rebind {
        typedef je_allocator_t<other_t> other;
    };

    pointer allocate(size_type n)
    {
        return (pointer) yesmalloc(n * sizeof(value_type));
    }

    void deallocate(pointer p, size_type)
    {
        yesfree(p);
    }

    template<typename p_t, typename... args_t>
    void construct (p_t* p, args_t&&... args)
    {
        ::new ((void *) p) p_t(std::forward<args_t>(args)...);
    }

    template<typename p_t>
    void destroy(p_t *p)
    {
        p->~p_t();
    }

    pointer address(reference x) const noexcept
    {
        return &x;
    }

    const_pointer address(const_reference x) const noexcept
    {
        return &x;
    }
};

}
