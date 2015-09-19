// Copyright (c) 2015 Urtashev Arslan. All rights reserved.
// Contacts: <urtashev@gmail.com>
//   
// Permission is hereby granted, free of charge, to any person obtaining
// a copy of this software and associated documentation files (the
// "Software"), to deal in the Software without restriction, including
// without limitation the rights to use, copy, modify, merge, publish,
// distribute, sublicense, and/or sell copies of the Software, and to
// permit persons to whom the Software is furnished to do so, subject to
// the following conditions:
//           
//   The above copyright notice and this permission notice shall be included
//   in all copies or substantial portions of the Software.
//              
// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,
// EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF
// MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.
// IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY
// CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT,
// TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE
// SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.

#ifndef GEO_BASE_MAPPED_MEMORY_GUARD_H_
#define GEO_BASE_MAPPED_MEMORY_GUARD_H_

#include <sys/mman.h>

#include "log.h"
#include "memory_units.h"

namespace geo_base {

class MappedMemoryGuard {
 public:
  explicit MappedMemoryGuard(void *addr = NULL, size_t length = 0) :
      addr_(addr),
      length_(length) {
    if (addr_)
      LogDebug("MappedMemoryGuard") << "Guard memory (" << addr_ << ", "
          << Megabytes(length_) << ")";
  }

  void unmap() {
    if (addr_) {
      LogDebug("MappedMemoryGuard") << "Unmap memory (" << addr_ << ", "
          << Megabytes(length_) << ")";
      munmap(addr_, length_);
      addr_ = NULL;
      length_ = 0;
    }
  }

  MappedMemoryGuard(MappedMemoryGuard&& m) :
      addr_(NULL),
      length_(0) {
    std::swap(addr_, m.addr_);
    std::swap(length_, m.length_);
  }

  MappedMemoryGuard& operator = (MappedMemoryGuard&& m) {
    std::swap(addr_, m.addr_);
    std::swap(length_, m.length_);
    return *this;
  }

  MappedMemoryGuard(const MappedMemoryGuard&) = delete;
  MappedMemoryGuard& operator = (const MappedMemoryGuard&) = delete;

  void* addr() const {
    return addr_;
  }

  size_t length() const {
    return length_;
  }

  ~MappedMemoryGuard() {
    unmap();
  }
  
 private:
  void *addr_;
  size_t length_;
};

} // namespace geo_base

#endif // GEO_BASE_MAPPED_MEMORY_GUARD_H_
