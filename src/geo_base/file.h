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

#ifndef GEO_BASE_FILE_H_
#define GEO_BASE_FILE_H_

#include "file_guard.h"

namespace geo_base {

// File is a handler of file descriptor. Needed for simple working
// with file descriptor in MemoryMappedFile. See memory_mapped_file.h.
// Coorect open/close file using RAII.
class File {
 public:
  // Open file for reading with O_RDONLY|O_CLOEXEC flags.
  void OpenReadOnly(const char* path);

  // Open file for reading and writing with O_RFWR|O_CREAT|O_CLOEXEC|O_TRUNC
  // flags and S_IWUSR|S_IRUSR|S_IRGRP|S_IROTH permissions.
  void OpenReadWrite(const char* path);

  // Return length of an open file in bytes. Throws exception if file is not
  // opened. Needed for right memory mapping in MemoryMappedFile
  size_t BytesCount() const;

  virtual ~File() { }

  int fd() const {
    return file_guard_.fd();
  }

private:
  FileGuard file_guard_;
};

} // namespace geo_base

#endif // GEO_BASE_FILE_H_
