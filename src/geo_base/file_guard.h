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

#ifndef GEO_BASE_FILE_GUARD_H_
#define GEO_BASE_FILE_GUARD_H_

#include <unistd.h>

#include "log.h"

namespace geo_base {

class FileGuard {
 public:
  explicit FileGuard(int fd = -1) :
      fd_(fd) {
    if (fd_ != -1)
      LogDebug("FileGuard") << "Guard file descriptor: " << fd_;
  }

  void close() {
    if (fd_ != -1) {
      LogDebug("FileGuard") << "Close file descriptor: " << fd_;
      ::close(fd_);
      fd_ = -1;
    }
  }

  FileGuard(FileGuard&& g) :
      fd_(-1) {
    std::swap(fd_, g.fd_);
  }

  FileGuard& operator = (FileGuard&& g) {
    std::swap(fd_, g.fd_);
    return *this;
  }

  FileGuard& operator = (const FileGuard&) = delete;
  FileGuard(const FileGuard&) = delete;

  ~FileGuard() {
    close();
  }

  int fd() const {
    return fd_;
  }

 private:
  int fd_;
};

} // namespace geo_base

#endif // GEO_BASE_FILE_GUARD_H_
