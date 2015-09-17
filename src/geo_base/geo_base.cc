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

#include "geo_base.h"

#include "common.h"
#include "geo_data_wrapper.h"

namespace geo_base {

class GeoBase::Impl {
 public:
  explicit Impl(const char* path) :
      wrapper(path) {
  }

  RegionID Lookup(const Location& location, GeoBase::LookupInfo* info) const {
    return GeoDataLookup(*wrapper.geo_data(), location, info);
  }

  const char* LookupValueByKey(RegionID region_id, const char* key) const {
    const GeoData* geo_data = wrapper.geo_data();

    const Region* regions = geo_data->regions;
    const Count regions_count = geo_data->regions_count;

    const Region* region = Find(regions, regions_count, region_id);
    if (!region)
      return NULL;

    const KeyValue* kvs = geo_data->kvs + region->kvs_offset;
    const Count kvs_count = region->kvs_count;

    const KeyValue* kv = Find(kvs, kvs_count, key, KeyValue::Cmp(geo_data->blobs));
    if (!kv)
      return NULL;

    return geo_data->blobs + kv->value;
  }

  template<typename Callback>
  void ForEachKeyValue(RegionID region_id, Callback callback) const {
    const GeoData* geo_data = wrapper.geo_data();

    const Region* regions = geo_data->regions;
    const Count regions_count = geo_data->regions_count;

    const Region* region = Find(regions, regions_count, region_id);
    if (!region)
      return;

    const KeyValue* kvs = geo_data->kvs + region->kvs_offset;
    const Count kvs_count = region->kvs_count;

    for (Count i = 0; i < kvs_count; ++i) {
      const char* key = geo_data->blobs + kvs[i].key;
      const char* value = geo_data->blobs + kvs[i].value;
      callback(key, value);
    }
  }

  uint32_t TouchMemory() const {
    return wrapper.GetSimpleChecksum();
  }

 private:
  GeoDataWrapper wrapper;
};

GeoBase::GeoBase(const char* path) {
  impl = new Impl(path);
}

GeoBase::~GeoBase() {
  delete impl;
}

RegionID GeoBase::Lookup(const Location& location,
    GeoBase::LookupInfo* info) const {
  return impl->Lookup(location, info);
}

const char* GeoBase::LookupValueByKey(RegionID region_id,
    const char* key) const {
  return impl->LookupValueByKey(region_id, key);
}

void GeoBase::ForEachKeyValue(RegionID region_id,
    KeyValueVisitor visitor) const {
  return impl->ForEachKeyValue(region_id, visitor);
}

uint32_t GeoBase::TouchMemory() const {
  return impl->TouchMemory();
}

} // namespace geo_base
