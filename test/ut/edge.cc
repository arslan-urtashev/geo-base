#include <gmock/gmock.h>

#include "edge.h"

using namespace geo_base;

class EdgeTest : public ::testing::Test {
 public:
  virtual void SetUp() {
  }

  virtual void TearDown() {
  }

  const Point* points() const {
    return points_.data();
  }

  const Edge MakeEdge(const Point& a, const Point& b) {
    points_.push_back(a);
    points_.push_back(b);
    return Edge(points_.size() - 2, points_.size() - 1);
  }

 private:
  std::vector<Point> points_;
};

TEST_F(EdgeTest, ContainsPoint) {
  Edge a = MakeEdge(Point(0, 0), Point(2, 2));

  EXPECT_TRUE(a.Contains(Point(0, 0), points()));
  EXPECT_TRUE(a.Contains(Point(1, 1), points()));
  EXPECT_TRUE(a.Contains(Point(2, 2), points()));

  EXPECT_FALSE(a.Contains(Point(-1, -1), points()));
  EXPECT_FALSE(a.Contains(Point(3, 3), points()));
  EXPECT_FALSE(a.Contains(Point(2, 1), points()));
}
