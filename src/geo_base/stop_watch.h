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

#ifndef GEO_BASE_STOP_WATCH_H_
#define GEO_BASE_STOP_WATCH_H_

#include "exception.h"

#include <sys/time.h>

namespace geo_base {

// StopWatch is a simple timer.
class StopWatch {
 public:
  StopWatch() {
    point = GetSeconds();
  }

  double CheckPoint() {
    double ret = GetSeconds() - point;
    point = GetSeconds();
    return ret;
  }

 private:
  static double GetSeconds() {
    struct timeval tv;
    if (gettimeofday(&tv, NULL) < 0)
      throw Exception("%s", strerror(errno));
    return (tv.tv_sec * 1000000 + tv.tv_usec) * 1.0 / 1000000;
  }

  double point;
};

}

#endif
