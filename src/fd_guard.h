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

#ifndef GEO_BASE_FDGUARD_H_
#define GEO_BASE_FDGUARD_H_

#include <unistd.h>

#include "log.h"

namespace geo_base {

struct FDGuard {
  int fd;

  explicit FDGuard(int fd = -1) :
      fd(fd) {
    Guard(fd);
  }

  void close() {
    if (fd != -1) {
      LogDebug("FDGuard") << "close fd = " << fd;
      ::close(fd);
      fd = -1;
    }
  }

  void Guard(int fd_) {
    close();
    fd = fd_;
    if (fd != -1)
      LogDebug("FDGuard") << "guard fd = " << fd;
  }

  ~FDGuard() {
    close();
  }

  FDGuard(FDGuard const &) = delete;
  FDGuard& operator = (FDGuard const &) = delete;
};

} // namespace geo_base

#endif // GEO_BASE_FDGUARD_H_
