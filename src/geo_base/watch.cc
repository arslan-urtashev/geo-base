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

#include "watch.h"

namespace geo_base {

static OutputStream& CheckOutput(OutputStream& out, int x) {
  out << (x < 10 ? "0" : "") << x;
  return out;
}

static OutputStream& operator << (OutputStream& out, const struct tm& tm) {
  CheckOutput(out, tm.tm_mday) << "-";
  CheckOutput(out, tm.tm_mon + 1) << "-";
  out << tm.tm_year + 1900 << " ";
  CheckOutput(out, tm.tm_hour) << ":";
  CheckOutput(out, tm.tm_min) << ":";
  CheckOutput(out, tm.tm_sec);
  return out;
}

OutputStream& operator << (OutputStream& out, const Watch&) {
  time_t t = time(&t);
  struct tm tm = *localtime_r(&t, &tm);
  out << "[" << tm << "] ";
  return out;
}


} // namespace geo_base
