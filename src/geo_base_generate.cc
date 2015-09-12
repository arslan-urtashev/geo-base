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

#include "geo_base_generate.h"

#include "hash.h"
#include "log.h"

namespace geo_base {

void GenerateContext::Fini(GeoBaseAllocator *base) {
  GeoData *geo_data = base->geo_data();

#define GEO_BASE_DEF_VAR(Var, var) \
  do { \
    geo_data->var = var; \
  } while (false);

#define GEO_BASE_DEF_PTR(Ptr, ptr) \
  // undef

#define GEO_BASE_DEF_ARR(Arr, arr) \
  do { \
    geo_data->arr##_count = arr.size(); \
    geo_data->arr = (Arr *) base->Allocate(sizeof(Arr) * arr.size()); \
    memcpy(geo_data->arr, arr.data(), sizeof(Arr) * arr.size()); \
    arr.clear(); \
    arr.shrink_to_fit(); \
  } while (false);

  GEO_BASE_DEF_GEO_DATA

#undef GEO_BASE_DEF_VAR
#undef GEO_BASE_DEF_PTR
#undef GEO_BASE_DEF_ARR

  geo_data->version = GeoDataVersion();
}

double GenerateContext::MemoryUsage() const
{
  double total = 0;

#define GEO_BASE_DEF_VAR(Var, var) \
  total += sizeof(Var);

#define GEO_BASE_DEF_PTR(Ptr, ptr) \
  // undef

#define GEO_BASE_DEF_ARR(Arr, arr) \
  total += sizeof(Count); \
  total += sizeof(Arr) * arr.size();

  GEO_BASE_DEF_GEO_DATA

#undef GEO_BASE_DEF_VAR
#undef GEO_BASE_DEF_PTR
#undef GEO_BASE_DEF_ARR

  return total * 1.0 / (1024. * 1024.);
}

static bool IsBadEdge(const Edge& e, const Point* p) {
  if (e.beg == e.end)
    return true;
  if (fabs(ConvertToDouble(p[e.beg].x - p[e.end].x)) > 300.0)
    return true;
  return false;
}

// Saving all points and represent polygon as is array of edges.
static void GenerateEdges(RegionID region_id, const std::vector<Point>& points,
    GenerateContext *ctx, std::vector<Edge> &edges) {
  edges.clear();
  edges.reserve(points.size());
  for (Ref i = 0; i < points.size(); ++i) {
    Ref j = (i + 1 == points.size() ? 0 : i + 1);
    Edge e(ctx->AddPoint(points[i]), ctx->AddPoint(points[j]));
    
    if (ctx->points[e.beg].x > ctx->points[e.end].x)
      std::swap(e.beg, e.end);

    if (IsBadEdge(e, ctx->points.data())) {
      if (e.beg != e.end)
        LogWarning("generate", region_id) <<  "bad edge detected = "
            << ctx->points[e.beg] << " -> " << ctx->points[e.end];
      continue;
    }
    
    edges.push_back(e);
  }
}

static void GenerateCheckPoints(const std::vector<Edge>& e, const Point* p,
    std::vector<CheckPoint> &checkpoints) {
  checkpoints.clear();
  checkpoints.reserve(2 * e.size());
  for (Ref i = 0; i < e.size(); ++i) {
    checkpoints.push_back({p[e[i].beg].x, i, false});
    checkpoints.push_back({p[e[i].end].x, i, true});
  }
  std::sort(checkpoints.begin(), checkpoints.end(),
    [&] (const CheckPoint& a, const CheckPoint& b) {
      return a.coordinate < b.coordinate;
    }
  );
}

static Square GetSquare(const std::vector<Point>& p) {
  Square s = 0;
  for (Ref i = 0; i < p.size(); ++i) {
    Ref j = (i + 1 == p.size() ? 0 : i + 1);
    s += p[i].CrossProduct(p[j]);
  }
  return s > 0 ? s : -s;
}

static uint64_t GetHash(RegionID region_id, const std::vector<Point>& points) {
  uint64_t hash = 0;
  hash ^= PolynomialHash<373>()(region_id);
  hash ^= PolynomialHash<337>()(points);
  return hash;
}

void GeoBaseGenerate::Update(RegionID region_id, PolygonID polygon_id,
    const std::vector<Point>& points, bool is_inner) {
  if (points.size() <= 2) {
    LogWarning("generate", region_id) << "Polygon to small!";
    return;
  }

  uint64_t hash = GetHash(region_id, points);
  if (ctx.processed.find(hash) != ctx.processed.end()) {
    LogError("generate", region_id) << "Region is already processed!";
    return;
  } else {
    ctx.processed.insert(hash);
  }

  Polygon polygon;
  polygon.region_id = region_id;
  polygon.polygon_id = polygon_id;
  polygon.square = GetSquare(points);
  polygon.is_inner = is_inner;
  
  polygon.rectangle.Init();
  for (const Point& p : points)
    polygon.rectangle.Relax(p);

  polygon.parts_offset = ctx.parts.size();
  
  std::vector<Edge> &edges = ctx.buf.edges;
  GenerateEdges(region_id, points, &ctx, edges);

  std::vector<CheckPoint> &checkpoints = ctx.buf.checkpoints;
  GenerateCheckPoints(edges, ctx.points.data(), checkpoints);

  std::vector<Edge> &erase = ctx.buf.erase;
  for (Ref l = 0, r = 0; l < checkpoints.size(); l = r) {
    r = l + 1;
    while (r < checkpoints.size() &&
        checkpoints[l].coordinate == checkpoints[r].coordinate)
      ++r;

    erase.clear();
    for (Ref i = l; i < r; ++i)
      if (checkpoints[i].erase)
        erase.push_back(edges[checkpoints[i].ref]);

    sort(erase.begin(), erase.end());

    Part part;
    part.edge_refs_offset = ctx.edge_refs.size();
    part.coordinate = checkpoints[l].coordinate;

    if (l > 0) {
      const Part& prev = ctx.parts.back();
      for (Ref i = prev.edge_refs_offset; i < part.edge_refs_offset; ++i)
        if (!binary_search(erase.begin(), erase.end(),
              ctx.edges[ctx.edge_refs[i]]))
          ctx.edge_refs.push_back(ctx.edge_refs[i]);
    }

    for (Ref i = l; i < r; ++i)
      if (!checkpoints[i].erase)
        if (!binary_search(erase.begin(), erase.end(),
              edges[checkpoints[i].ref]))
          ctx.edge_refs.push_back(ctx.AddEdge(edges[checkpoints[i].ref]));

    sort(ctx.edge_refs.begin() + part.edge_refs_offset, ctx.edge_refs.end(),
      [&] (const Ref& a, const Ref& b) {
        const Edge& e1 = ctx.edges[a];
        const Edge& e2 = ctx.edges[b];
        return e1.Lower(e2, ctx.points.data());
      }
    );

    ctx.parts.push_back(part);
  }

  polygon.parts_count = ctx.parts.size() - polygon.parts_offset;
  ctx.polygons.push_back(polygon);
}

static bool IsInnerPolygon(const proto::Polygon& p) {
  return p.type() == proto::Polygon::TYPE_INNER;
}

void GeoBaseGenerate::Update(RegionID region_id,
    const proto::Polygon& polygon) {
  std::vector<Point> &points = ctx.buf.points;

  std::vector<Location> &locations = ctx.buf.locations;
  locations.clear();
  for (const proto::Location& l : polygon.locations())
    locations.push_back(Location(l.lon(), l.lat()));

  processor(locations, [&] (const std::vector<Location>& locations) {
    points.clear();
    for (const Location& l : locations)
      points.push_back(Point(l.lon, l.lat));
    Update(region_id, polygon.polygon_id(), points, IsInnerPolygon(polygon));
  });
}

void GeoBaseGenerate::Update(const proto::Region& data) {
  for (const proto::Polygon& p : data.polygons())
    Update(data.region_id(), p);

  Region region;
  region.region_id = data.region_id();
  region.kvs_offset = ctx.kvs.size();

  for (const proto::KeyValue& x : data.kvs()) {
    KeyValue kv;
    kv.key = ctx.AddBlob(x.key());
    kv.value = ctx.AddBlob(x.value());
    ctx.kvs.push_back(kv);
  }

  region.kvs_count = ctx.kvs.size() - region.kvs_offset;
  std::sort(ctx.kvs.begin() + region.kvs_offset, ctx.kvs.end(),
      KeyValue::Cmp(ctx.blobs.data()));

  ctx.regions.push_back(region);
}

void GeoBaseGenerate::GenerateBoxes() {
  LogInfo("generate") << "Generate boxes...";

  for (Coordinate x0 = AreaBox::kLowerX; x0 < AreaBox::kUpperX; x0 += AreaBox::kDeltaX) {
    for (Coordinate y0 = AreaBox::kLowerY; y0 < AreaBox::kUpperY; y0 += AreaBox::kDeltaY) {
      AreaBox box;
      box.polygon_refs_offset = ctx.polygon_refs.size();

      for (Ref i = 0; i < ctx.polygons.size(); ++i) {
        Rectangle r(x0, y0, x0 + AreaBox::kDeltaX, y0 + AreaBox::kDeltaY);
        if (ctx.polygons[i].rectangle.HasIntersections(r))
          ctx.polygon_refs.push_back(i);
      }

      box.polygon_refs_count = ctx.polygon_refs.size() - box.polygon_refs_offset;

      std::sort(ctx.polygon_refs.begin() + box.polygon_refs_offset, ctx.polygon_refs.end(),
        [&] (const Ref& a, const Ref& b) {
          const Polygon* p = ctx.polygons.data();
          return p[a].region_id < p[b].region_id ||
              (p[a].region_id == p[b].region_id && p[a].is_inner && !p[b].is_inner);
        }
      );

      ctx.boxes.push_back(box);
    }
  }

  LogInfo("generate") << "Boxes generated";
}

void GeoBaseGenerate::Save() {
  GenerateBoxes();

  std::sort(ctx.regions.begin(), ctx.regions.end());
  for (Region& r : ctx.regions)
    r.square = 0;

  // Compute regions total square
  for (const Polygon& p : ctx.polygons) {
    Region* region = Find(ctx.regions.data(), ctx.regions.size(), p.region_id);
    if (region)
      region->square += p.square;
  }

  // Create fake part for right edge_refs_count calculation.
  Part fake_part;
  fake_part.coordinate = 0;
  fake_part.edge_refs_offset = ctx.edge_refs.size();
  ctx.parts.push_back(fake_part);
  
  ctx.Fini(&base);
}

} // namespace geo_base
