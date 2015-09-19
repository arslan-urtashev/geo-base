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

#include "geo_data.h"

#include "exception.h"
#include "hash.h"
#include "io.h"
#include "memory_units.h"

namespace geo_base {

void GeoDataSave(void *dat, GeoData* geo_data) {
  geo_data->version = GeoDataVersion();

#define GEO_BASE_DEF_VAR(Var, var)
  // undef

// Convert all pointers in GeoData as offsets in data file.
#define GEO_BASE_DEF_PTR(Ptr, ptr) \
  if (geo_data->ptr) \
    geo_data->ptr = (Ptr*) (((char *) geo_data->ptr) - ((ptrdiff_t) dat));

#define GEO_BASE_DEF_ARR(Arr, arr) \
  GEO_BASE_DEF_PTR(Arr, arr);

  GEO_BASE_DEF_GEO_DATA

#undef GEO_BASE_DEF_VAR
#undef GEO_BASE_DEF_PTR
#undef GEO_BASE_DEF_ARR
}

void GeoDataMap(void *dat, GeoData *geo_data) {
#define GEO_BASE_DEF_VAR(Var, var)
  // undef

// Convert GeoData pointers to valid state.
#define GEO_BASE_DEF_PTR(Ptr, ptr) \
  if (geo_data->ptr) \
    geo_data->ptr =  (Ptr*) (((ptrdiff_t) dat) + ((char *) geo_data->ptr))

#define GEO_BASE_DEF_ARR(Arr, arr) \
  GEO_BASE_DEF_PTR(Arr, arr);

  GEO_BASE_DEF_GEO_DATA

#undef GEO_BASE_DEF_PTR
#undef GEO_BASE_DEF_VAR
#undef GEO_BASE_DEF_ARR

  if (geo_data->version != GeoDataVersion())
    throw Exception("Wrong geo_data version, expect: %lu, got: %lu",
        GeoDataVersion(), geo_data->version);
}

static bool PointInPolygon(const Point& point, const Polygon& polygon,
    const GeoData& geo_data) {
  return polygon.Contains(point, geo_data.parts, geo_data.edge_refs,
      geo_data.edges, geo_data.points);
}

static bool AnswerIsBetter(const Polygon& answer, const Polygon& polygon,
    const GeoData& geo_data) {
  return answer.Better(polygon, geo_data.regions, geo_data.regions_count);
}

RegionID GeoDataLookup(const GeoData& geo_data, const Location& location,
    LookupInfo *info) {
  const Point point(location);

  // Determine in what AreaBox is the point.
  const Ref box_x = (point.x - AreaBox::kLowerX) / AreaBox::kDeltaX;
  const Ref box_y = (point.y - AreaBox::kLowerY) / AreaBox::kDeltaY;
  const Ref box_ref = box_x * AreaBox::kCountY + box_y;

  if (box_ref >= geo_data.boxes_count)
    return -1;

  const Count refs_offset = geo_data.boxes[box_ref].polygon_refs_offset;
  const Count refs_end = geo_data.boxes[box_ref].polygon_refs_count + refs_offset;

  const Polygon* answer = NULL;

  if (info)
    info->regions.clear();

  for (Ref l = refs_offset, r = 0; l < refs_end; l = r) {
    const Ref *refs = geo_data.polygon_refs;
    const Polygon* p = geo_data.polygons;

    r = l + 1;

    if (PointInPolygon(point, p[refs[l]], geo_data)) {
      if (p[refs[l]].is_inner) {
        // All polygons with same region_id must be skipped if polygon is inner
        // In GeoData inner polygons stores before outer polygons.
        while (r < refs_end && p[refs[l]].region_id == p[refs[r]].region_id)
          ++r;

      } else {
        if (!answer || !AnswerIsBetter(*answer, p[refs[l]], geo_data))
          answer = &p[refs[l]];

        if (info)
          info->regions.push_back(p[refs[l]].region_id);
      }
    }
  }

  if (info) {
    std::sort(info->regions.begin(), info->regions.end(),
      [&] (const RegionID& a, const RegionID& b) {
        const Region* r1 = Find(geo_data.regions, geo_data.regions_count, a);
        const Region* r2 = Find(geo_data.regions, geo_data.regions_count, b);
        return r1->Better(*r2);
      }
    );

    if (answer)
      info->polygon_id = answer->polygon_id;
  }

  return answer ? answer->region_id : -1;
}

void GeoDataShow(const GeoData& geo_data, OutputStream& out) {
  size_t total = 0;

  out << "geo_data:" << '\n';

#define GEO_BASE_DEF_VAR(Var, var) \
  out << "  " << #var << " = " << geo_data.var << '\n'; \
  total += sizeof(Var);

#define GEO_BASE_DEF_PTR(Ptr, ptr) \
  // undef

#define GEO_BASE_DEF_ARR(Arr, arr) \
  out \
    << "  " \
    << #arr \
      << " = "  \
    << geo_data.arr##_count << " x " << sizeof(Arr) \
      << " = " \
    << geo_data.arr##_count * sizeof(Arr) \
      << " = " \
    << Megabytes(geo_data.arr##_count * sizeof(Arr)) << '\n' \
  ; \
  total += sizeof(Arr) * geo_data.arr##_count;

  GEO_BASE_DEF_GEO_DATA

#undef GEO_BASE_DEF_VAR
#undef GEO_BASE_DEF_PTR
#undef GEO_BASE_DEF_ARR

  out << "Total = " << Gigabytes(total) << '\n';
}

static void UpdateVersion(const char* str, const Count sof, const Count pos,
    Version* version) {
  (*version) ^= PolynomialHash<>()(str, strlen(str));
  (*version) ^= PolynomialHash<>()(pos);
  (*version) ^= PolynomialHash<>()(sof);
}

Version GeoDataVersion() {
  Version version = 0;
  Count pos = 0;

#define GEO_BASE_DEF_VAR(Var, var) \
  UpdateVersion(#Var, sizeof(Var), ++pos, &version); \
  UpdateVersion(#var, 0, ++pos, &version);

#define GEO_BASE_DEF_PTR(Ptr, ptr) \
  UpdateVersion(#Ptr, sizeof(Ptr), ++pos, &version); \
  UpdateVersion(#ptr, 0, ++pos, &version);

#define GEO_BASE_DEF_ARR(Arr, arr) \
  GEO_BASE_DEF_VAR(Count, arr##_count); \
  GEO_BASE_DEF_PTR(Arr, arr);

  GEO_BASE_DEF_GEO_DATA

#undef GEO_BASE_DEF_VAR
#undef GEO_BASE_DEF_PTR
#undef GEO_BASE_DEF_ARR

  return version;
}

} // namespace geo_base
