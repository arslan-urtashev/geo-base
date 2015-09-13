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

#ifndef GEO_BASE_KEY_VALUE_H_
#define GEO_BASE_KEY_VALUE_H_

#include "common.h"

namespace geo_base {

// KeyValue contains offsets on GeoData.blobs, because all strings saving in
// GeoData.blobs without copy (in only one instance).
struct KeyValue {
  Count key;
  Count value;

  struct Cmp {
    const char* blobs;

    explicit Cmp(const char *blobs) :
        blobs(blobs) {
    }

    bool operator () (const KeyValue& a, const KeyValue& b) const {
      const char* k1 = blobs + a.key;
      const char *k2 = blobs + b.key;
      return strcmp(k1, k2) < 0;
    }

    bool operator () (const KeyValue& a, const char* s) const {
      const char* k = blobs + a.key;
      return strcmp(k, s) < 0;
    }

    bool operator () (const char* s, const KeyValue& a) const {
      const char* k = blobs + a.key;
      return strcmp(s, k) < 0;
    }
  };
};

} // namespace geo_base

#endif // GEO_BASE_KEY_VALUE_H_
