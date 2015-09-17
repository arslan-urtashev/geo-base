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

#ifndef GEO_BASE_GEO_DATA_H_
#define GEO_BASE_GEO_DATA_H_

#include "area_box.h"
#include "common.h"
#include "edge.h"
#include "geo_base.h"
#include "io.h"
#include "key_value.h"
#include "location.h"
#include "part.h"
#include "point.h"
#include "polygon.h"
#include "region.h"

namespace geo_base {

#define GEO_BASE_DEF_VAR(Var, var) \
  Var var;

#define GEO_BASE_DEF_PTR(Ptr, ptr) \
  Ptr *ptr;

#define GEO_BASE_DEF_ARR(Arr, arr) \
  GEO_BASE_DEF_VAR(Count, arr##_count); \
  GEO_BASE_DEF_PTR(Arr, arr);

#define GEO_BASE_DEF_GEO_DATA \
  GEO_BASE_DEF_VAR(Version, version); \
  GEO_BASE_DEF_ARR(Point, points); \
  GEO_BASE_DEF_ARR(Edge, edges); \
  GEO_BASE_DEF_ARR(Ref, edge_refs); \
  GEO_BASE_DEF_ARR(Part, parts); \
  GEO_BASE_DEF_ARR(Polygon, polygons); \
  GEO_BASE_DEF_ARR(Ref, polygon_refs); \
  GEO_BASE_DEF_ARR(AreaBox, boxes); \
  GEO_BASE_DEF_ARR(char, blobs); \
  GEO_BASE_DEF_ARR(KeyValue, kvs); \
  GEO_BASE_DEF_ARR(Region, regions);

// GeoData is a data type, which stores on disk after generating by
// GeoBaseGenerate. GeoBase uses GeoData for lookup region and other operations
// with geographical data. Declaration implemented as list of #define for use
// reflection for mapping/saving and other operations. See geo_data.cc for
// reflection example.
struct GeoData {
  GEO_BASE_DEF_GEO_DATA
};

#undef GEO_BASE_DEF_VAR
#undef GEO_BASE_DEF_PTR
#undef GEO_BASE_DEF_ARR

// Version of current GeoData declaration. Implemented as a simple 
// hash function.
Version GeoDataVersion();

// Make GeoData pointers as offsets in file with data. It is necessary because
// after unmap/map GeoData pointers will be invalid.
void GeoDataSave(void *dat, GeoData* geo_data);

// Mapping of GeoData pointers. Inverse of GeoDataSave operation. Restore valid
// pointers in data file.
void GeoDataMap(void *dat, GeoData* geo_data);

// Show GeoData sizes of arrays, values of variables and memory usage.
void GeoDataShow(const GeoData& geo_data, OutputStream& out);

typedef GeoBase::LookupInfo LookupInfo;

// Just fast search RegionID by locations. Stores debug data in
// LookupDebug (if it's not NULL).
RegionID GeoDataLookup(const GeoData& geo_data, const Location& location,
    LookupInfo* info = NULL);

} // namespace geo_base

#endif // GEO_BASE_GEO_DATA_H_
