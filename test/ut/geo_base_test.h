#include <gmock/gmock.h>

#include "edge.h"
#include "part.h"
#include "geo_data.h"

namespace geo_base {

class GeoDataTest {
#define GEO_BASE_DEF_VAR(Var, var) \
 protected: \
  Var var##_; \
 public: \
  const Var& var() const { \
    return var##_; \
  }

#define GEO_BASE_DEF_ARR(Arr, arr) \
 protected: \
  std::vector<Arr> arr##_; \
 public: \
  const Arr* arr() const { \
    return arr##_.data(); \
  }

  GEO_BASE_DEF_GEO_DATA

#undef GEO_BASE_DEF_VAR
#undef GEO_BASE_DEF_ARR
};

struct PartHelper : public Part {
  Count edge_refs_count;

  bool Contains(const Point& p, const GeoDataTest& g) const {
    return Part::Contains(p, edge_refs_count, g.edge_refs(),
        g.edges(), g.points());
  }
};

class GeoBaseTest : public GeoDataTest, public ::testing::Test {
 public:
  Edge MakeEdge(const Point& a, const Point& b) {
    points_.push_back(a);
    points_.push_back(b);
    return Edge(points_.size() - 2, points_.size() - 1);
  }

  PartHelper MakePart(Coordinate coordinate, const std::vector<Edge>& edges) {
    PartHelper helper;
    helper.coordinate = coordinate;
    helper.edge_refs_offset = edge_refs_.size();

    for (const Edge& e : edges) {
      edge_refs_.push_back(edges_.size());
      edges_.push_back(e);
    }

    helper.edge_refs_count = edge_refs_.size();

    std::sort(edge_refs_.begin() + helper.edge_refs_offset, edge_refs_.end(),
      [&] (const Ref& a, const Ref& b) {
        const Edge& e1 = edges_[a];
        const Edge& e2 = edges_[b];
        return e1.Lower(e2, points_.data());
      }
    );

    return helper;
  }

  const GeoDataTest& geo_data() const {
    const GeoDataTest* data = dynamic_cast<const GeoDataTest*>(this);
    assert(data != nullptr);
    return *data;
  }
};

}
