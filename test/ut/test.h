#include <gmock/gmock.h>

#include "edge.h"

namespace geo_base {

class Test : public ::testing::Test {
 public:
  const Edge MakeEdge(const Point& a, const Point& b) {
    points_.push_back(a);
    points_.push_back(b);
    return Edge(points_.size() - 2, points_.size() - 1);
  }

  const Point* points() const {
    return points_.data();
  }

 private:
  std::vector<Point> points_;
};

}
