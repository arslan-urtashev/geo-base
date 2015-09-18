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

#include "exception.h"
#include "get_opt.h"

#include <functional>
#include <string>

#include <getopt.h>
#include <stdlib.h>

namespace geo_base {

static int ParseInt(const char* str) {
  char* endptr = NULL;
  int ret = strtol(str, &endptr, 10);
  if (*endptr)
    throw Exception("Can't ParseInt(): %s", str);
  return ret;
}

typedef std::function<void (const char*)> GetOptCallback;

void ParseOptions(int argc, char* argv[], Options* opts) {
  const struct option options[] = {
    { "jobs", required_argument, NULL, 0 },
    { "touch-memory", no_argument, NULL, 0 },
    { NULL, 0, NULL, 0 }
  };

  const GetOptCallback callbacks[] = {
    [&] (const char* arg) {
      opts->jobs = ParseInt(arg);
    },
    [&] (const char*) {
      opts->touch_memory = true;
    },
    [&] (const char*) {
      throw Exception("%s", "Unknown options!");
    }
  };

  int ch = 0, option_index = 0;
  while ((ch = getopt_long(argc, argv, "j:", options, &option_index)) != -1) {
    switch (ch) {
      case 0:
        callbacks[option_index](optarg);
        break;
      case 'j':
        opts->jobs = ParseInt(optarg);
        break;
      default:
        throw Exception("%s", "Unknown options!");
    }
  }

  while (optind < argc)
    opts->args.push_back(std::string(argv[optind++]));
}

} // namespace geo_base
