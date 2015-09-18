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

#ifndef GEO_BASE_MAPPED_MEMORY_ALLOCATOR_H_
#define GEO_BASE_MAPPED_MEMORY_ALLOCATOR_H_

#include "common.h"
#include "log.h"
#include "memory_mapped_file.h"

#include <unistd.h>

namespace geo_base {

// A MappedMemoryAllocator is a allocator, which returns space from memory mapped file.
// Allocated memory is aigned by kAlignment (may be necessary for SSE)
class MappedMemoryAllocator : public MemoryMappedFile {
 public:
  static const uint64_t kAlignment = 32;

  explicit MappedMemoryAllocator(const char* path) :
      offset_(0) {
    ReadWriteOpen(path);
    if (((ptrdiff_t) addr()) % kAlignment != 0)
      LogWarning("MappedMemoryAllocator") << "Mapped memory is not aligned!";
  }

  void *Allocate(size_t count) {
    size_t new_offset = Align(offset_ + count);
    if (ftruncate(fd(), new_offset) < 0)
      throw Exception("%s", strerror(errno));
    void *memory = ((char *) addr()) + offset_;
    offset_ = new_offset;
    return memory;
  }

 private:
  static size_t Align(size_t x) {
    while (x % kAlignment != 0)
      ++x;
    return x;
  }

  size_t offset_;
};

} // namespace geo_baes

#endif // MAPPED_MEMORY_ALLOCATOR_H_
