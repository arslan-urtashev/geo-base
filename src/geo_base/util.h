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

#ifndef GEO_BASE_UTIL_H_
#define GEO_BASE_UTIL_H_

#include "common.h"
#include "exception.h"
#include "io.h"
#include "location.h"
#include "log.h"
#include "point.h"
#include "stop_watch.h"

#include "proto/region.pb.h"

#include <cmath>
#include <functional>
#include <mutex>
#include <sstream>
#include <string>
#include <vector>

#include <arpa/inet.h>

namespace geo_base {

// Parse protobuf input with geographical regions and call callback
// for each region. Thread unsafe.
class ProtoParser {
 public:
  explicit ProtoParser(int fd) :
      fd_(fd) {
  }

  template<typename Callback>
  void operator () (Callback callback) {
    proto::Region region;
    std::vector<char> buffer;

    uint32_t count = 0;
    while (Read(fd_, (char*) &count, sizeof(count))) {
      count = ntohl(count);

      buffer.resize(count);
      if (!Read(fd_, buffer.data(), buffer.size()))
        throw Exception("%s", "Can't read data");

      if (!region.ParseFromArray(buffer.data(), buffer.size()))
        throw Exception("%s", "Can't parse region");

      callback(region);
    }
  }

 private:
  int fd_;
};

// Writes std::string buffer to fd output in right format. Thread safe.
// If buffer is a serialized region protobuf, then region can be readed
// with ProtoParser.
class ProtoWriter {
 public:
  explicit ProtoWriter(int fd) :
      fd_(fd) {
  }

  void Write(const std::string& buffer) {
    uint32_t count = htonl(static_cast<uint32_t>(buffer.size()));

    std::lock_guard<std::mutex> lock(mutex_);

    geo_base::Write(fd_, (const char*) &count, sizeof(count));
    geo_base::Write(fd_, buffer.data(), buffer.size());
  }

 private:
  int fd_;
  std::mutex mutex_;
};

// Preprocess polygon locations. Try to detect multipolygons in input
// geographical data.
class PolygonProcessor {
 public:
  template<typename Callback>
  void operator () (const std::vector<Location>& raw_locations,
      Callback callback) {
    locations.clear();
    for (const Location& l : raw_locations) {
      if (locations.empty() || Point(locations.back()) != Point(l))
        locations.push_back(l);

      if (locations.size() >= 3) {
        size_t n = locations.size();
        if (OnOneLine(locations[n - 3], locations[n - 2], locations[n - 1])) {
          std::swap(locations[n - 2], locations[n - 1]);
          locations.pop_back();
        }
      }

      if (locations.size() > 3 && 
          Point(locations.front()) == Point(locations.back())) {
        locations.pop_back();
        callback(locations);
        locations.clear();
      }
    }

    if (locations.size() > 2)
      callback(locations);
  }

 private:
  static bool OnOneLine(const Location& a, const Location& b, const Location& c) {
    Point p1(a);
    Point p2(b);
    Point p3(c);
    return (p2 - p1).CrossProduct(p3 - p1) == 0;
  }

  std::vector<Location> locations;
};

} // namespace geo_base

#endif // GEO_BASE_UTIL_H_
