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

#ifndef GEO_BASE_TOOLS_GET_OPT_H_
#define GEO_BASE_TOOLS_GET_OPT_H_

#include "geo_base/common.h"

#include <vector>
#include <string>

namespace geo_base {

struct Options {
  Count jobs;
  bool touch_memory;
  Count random;

  Options() :
      jobs(2),
      touch_memory(false),
      random(0) {
  }

  std::vector<std::string> args;
};

void ParseOptions(int argc, char* argv[], Options* opts);

inline Options GetOpts(int argc, char* argv[]) {
  Options opts;
  ParseOptions(argc, argv, &opts);
  return opts;
}

} // namespace geo_base

#endif // GEO_BASE_TOOLS_GET_OPT_H_