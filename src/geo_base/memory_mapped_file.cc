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

#include "memory_mapped_file.h"

#include "exception.h"

#include <cassert>

#include <sys/mman.h>

namespace geo_base {

void MemoryMappedFile::ReadOnlyOpen(const char* path) {
  File::ReadOnlyOpen(path);
  
  size_t length = SizeOfOpenFile();
  void *addr = mmap(NULL, length, PROT_READ, MAP_SHARED, fd(), 0);
  if (addr == MAP_FAILED)
    throw Exception("MemoryMappedFile.ReadOnlyOpen: %s", strerror(errno));

  mmap_guard.Guard(addr, length);
}

void MemoryMappedFile::ReadWriteOpen(const char* path,
    size_t memory_size /* = kDefaultMemorySize */) {
  File::ReadWriteOpen(path);

  void *addr = mmap(NULL, memory_size, PROT_READ | PROT_WRITE,
      MAP_SHARED, fd(), 0);

  if (addr == MAP_FAILED)
    throw Exception("MemoryMappedFile.ReadWriteOpen: %s", strerror(errno));

  mmap_guard.Guard(addr, memory_size);
}

uint8_t MemoryMappedFile::GetSimpleChecksum() const {
  assert(SizeOfOpenFile() % (2 * sizeof(uint64_t)) == 0);

  const uint64_t* ptr = static_cast<const uint64_t*>(addr());
  const uint64_t length = SizeOfOpenFile() / sizeof(uint64_t);

  uint8_t result = 0;

  for (uint64_t i = 0; i < length; ++i) {
    result = static_cast<uint64_t>(result) + *ptr;
    ++ptr;
  }

  return result;
}

} // namespace geo_base
