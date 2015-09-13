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

#ifndef GEO_BASE_GEO_BASE_ALLOCATOR_H_
#define GEO_BASE_GEO_BASE_ALLOCATOR_H_

#include "geo_data.h"
#include "mapped_memory_allocator.h"

namespace geo_base {

// GeoBaseAllocator is a extended MappedMemoryAllocator, which allocates
// GeoData in constructor and saves GeoData in destructor (it is necessary
// for the correct replacement of pointers in the data, see geo_data.cc).
class GeoBaseAllocator : public MappedMemoryAllocator {
 public:
  explicit GeoBaseAllocator(const char* path) :
      MappedMemoryAllocator(path) {
    geo_data_ = (GeoData*) Allocate(sizeof(GeoData));
  }

  GeoData* geo_data() const {
    return geo_data_;
  }

  void Show(OutputStream& out) const {
    GeoDataShow(*geo_data_, out);
  }

  ~GeoBaseAllocator() {
    GeoDataSave(addr(), geo_data_);
  }

 private:
  GeoData* geo_data_;
};

} // namespace geo_base

#endif // GEO_BASE_GEO_BASE_ALLOCATOR_H_
