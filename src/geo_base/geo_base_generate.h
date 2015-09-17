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

#ifndef GEO_BASE_GENERATE_H_
#define GEO_BASE_GENERATE_H_

#include "common.h"
#include "geo_base_allocator.h"
#include "geo_data.h"
#include "hash.h"

#include <unordered_set>
#include <unordered_map>

namespace geo_base {

// CheckPoint A checkpoint is a point in which persistent scanline data 
// structure will be updated.
struct CheckPoint {
  // Coordinate is a variable on which GeoDataLookup will search the 
  // correct version of the data structure.
  Coordinate coordinate;
  
  // Ref refers on edge in current edges array (see geo_base_generate.cc 
  // for details).
  Ref ref;

  // if true, then edge ends in this CheckPoint.
  // if false, then edge is stats in this CheckPoint.
  bool erase;
};

#define GEO_BASE_DEF_VAR(Var, var) \
  Var var;

#define GEO_BASE_DEF_PTR(Ptr, ptr) \
  // undef

#define GEO_BASE_DEF_ARR(Arr, arr) \
  std::vector<Arr> arr;

// Context of Generate state. Contains all data of state.
struct GenerateContext {
  GEO_BASE_DEF_GEO_DATA

  // Save point in GeoData without duplicates.
  Ref AddPoint(const Point& p);

  // Save edge in GeoData without duplacates.
  Ref AddEdge(const Edge& e);

  // Save blob in GeoData withtout duplicates.
  Count AddBlob(const std::string& b);

  void Fini(GeoBaseAllocator *base);

  double MemoryUsage() const;

  struct {
    std::vector<CheckPoint> checkpoints;
    std::vector<Edge> edges;
    std::vector<Edge> erase;
    std::vector<Point> points;
    std::vector<Location> locations;
  } buf;

  struct {
    std::unordered_map<Edge, Ref, Hash64> edges;
    std::unordered_map<Point, Ref, Hash64> points;
    std::unordered_map<std::string, Count> blobs;
  } saved;

  std::unordered_set<uint64_t> processed;
};

#undef GEO_BASE_DEF_VAR
#undef GEO_BASE_DEF_PTR
#undef GEO_BASE_DEF_ARR

class GeoBaseGenerate {
 public:
  explicit GeoBaseGenerate(const char* path) :
      base(path) {
  }

  // Update current generate state. Generate persirtent scanlines for
  // all polygons in given region.
  void Update(const proto::Region& proto);

  // Save current state on disk.
  void Save();

  void ShowBase(OutputStream& out) const {
    base.Show(out);
  }

  const GenerateContext& Context() const {
    return ctx;
  }

 private:
  void Update(RegionID region_id, const proto::Polygon& polygon);
  void Update(RegionID region_id, PolygonID polygon_id, const std::vector<Point>& points, bool is_inner);

  void GenerateBoxes();

  GeoBaseAllocator base;
  GenerateContext ctx;
  PolygonProcessor processor;
};

} // namespace geo_base

#endif // GEO_BASE_GENERATE_H_
