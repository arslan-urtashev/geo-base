// copyright (c) 2015 urtashev arslan. all rights reserved.
// contacts: <urtashev@gmail.com>
//
// permission is hereby granted, free of charge, to any person obtaining a copy of this software
// and associated documentation files (the "software"), to deal in the software without
// restriction, including without limitation the rights to use, copy, modify, merge, publish,
// distribute, sublicense, and/or sell copies of the software, and to permit persons to whom the
// software is furnished to do so, subject to the following conditions:
//
//   the above copyright notice and this permission notice shall be included in all copies or
//   substantial portions of the software.
//
// the software is provided "as is", without warranty of any kind, express or implied, including
// but not limited to the warranties of merchantability, fitness for a particular purpose and
// noninfringement. in no event shall the authors or copyright holders be liable for any claim,
// damages or other liability, whether in an action of contract, tort or otherwise, arising from,
// out of or in connection with the software or the use or other dealings in the software.

#pragma once

#include <geo_base/util/algo.h>
#include <geo_base/util/log.h>

#include <sys/mman.h>

namespace geo_base {

class mem_guard_t {
public:
    mem_guard_t()
        : addr_(nullptr)
        , length_(0)
    { }

    mem_guard_t(void *addr, size_t length)
        : addr_(addr)
        , length_(length)
    {
        if (addr_)
            log_debug("Guard memory %p, %lu", addr_, length_);
    }

    mem_guard_t(mem_guard_t &&g)
        : mem_guard_t()
    {
        std::swap(addr_, g.addr_);
        std::swap(length_, g.length_);
    }

    mem_guard_t &operator = (mem_guard_t&& g)
    {
        std::swap(addr_, g.addr_);
        std::swap(length_, g.length_);
        return *this;
    }

    void *data() const
    {
        return addr_;
    }

    size_t size() const
    {
        return length_;
    }

    ~mem_guard_t()
    {
        if (addr_) {
            log_debug("Unmap memory %p, %lu", addr_, length_);
            munmap(addr_, length_);
        }
    }

private:
    void *addr_;
    size_t length_;

    GEO_BASE_DISALLOW_EVIL_CONSTRUCTORS(mem_guard_t);
};

} // namespace geo_base
