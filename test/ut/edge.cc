#include <gmock/gmock.h>

#include "edge.h"

using namespace geo_base;

class EdgeTest : public ::testing::Test {
 public:
  virtual void SetUp() {
  }

  virtual void TearDown() {
    points_.clear();
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

TEST_F(EdgeTest, EdgeLower) {
  Edge a = MakeEdge(Point(0, 0), Point(2, 1));
  Edge b = MakeEdge(Point(0, 0), Point(2, 2));
  Edge c = MakeEdge(Point(1, 3), Point(5, 5));
  Edge d = MakeEdge(Point(0, 4), Point(5, 5));

  EXPECT_TRUE(a.Lower(b, points()));
  EXPECT_TRUE(a.Lower(c, points()));
  EXPECT_TRUE(a.Lower(d, points()));
  EXPECT_TRUE(b.Lower(c, points()));
  EXPECT_TRUE(b.Lower(d, points()));
  EXPECT_TRUE(c.Lower(d, points()));

  EXPECT_FALSE(d.Lower(a, points()));
  EXPECT_FALSE(d.Lower(b, points()));
  EXPECT_FALSE(d.Lower(c, points()));
  EXPECT_FALSE(c.Lower(b, points()));
  EXPECT_FALSE(c.Lower(a, points()));
  EXPECT_FALSE(b.Lower(a, points()));

  Edge a1 = MakeEdge(Point(0, 2), Point(3, 3));
  Edge a2 = MakeEdge(Point(0, 0), Point(1, 2));
  Edge a3 = MakeEdge(Point(0, 0), Point(3, 3));

  EXPECT_TRUE(a3.Lower(a2, points()));
  EXPECT_TRUE(a3.Lower(a1, points()));
  EXPECT_TRUE(a2.Lower(a1, points()));

  EXPECT_FALSE(a2.Lower(a3, points()));
  EXPECT_FALSE(a1.Lower(a3, points()));
  EXPECT_FALSE(a1.Lower(a2, points()));

  Edge a4 = MakeEdge(Point(2, 5), Point(3, 7));
  Edge a5 = MakeEdge(Point(2, 5), Point(4, 6));
  Edge a6 = MakeEdge(Point(2, 4), Point(4, 6));
  Edge a11 = MakeEdge(Point(3, 7), Point(10, 5));

  EXPECT_TRUE(a6.Lower(a5, points()));
  EXPECT_TRUE(a6.Lower(a4, points()));
  EXPECT_TRUE(a6.Lower(a11, points()));
  EXPECT_TRUE(a5.Lower(a4, points()));
  EXPECT_TRUE(a5.Lower(a11, points()));

  EXPECT_FALSE(a5.Lower(a6, points()));
  EXPECT_FALSE(a4.Lower(a6, points()));
  EXPECT_FALSE(a11.Lower(a6, points()));
  EXPECT_FALSE(a4.Lower(a5, points()));
  EXPECT_FALSE(a11.Lower(a5, points()));

  Edge a7 = MakeEdge(Point(4, 4), Point(5, 5));
  Edge a8 = MakeEdge(Point(5, 5), Point(6, 4));
  Edge a9 = MakeEdge(Point(5, 4), Point(6, 3));
  Edge a10 = MakeEdge(Point(4, 4), Point(6, 3));

  EXPECT_TRUE(a10.Lower(a9, points()));
  EXPECT_TRUE(a10.Lower(a7, points()));
  EXPECT_TRUE(a10.Lower(a8, points()));
  EXPECT_TRUE(a9.Lower(a8, points()));

  EXPECT_FALSE(a9.Lower(a10, points()));
  EXPECT_FALSE(a7.Lower(a10, points()));
  EXPECT_FALSE(a8.Lower(a10, points()));
  EXPECT_FALSE(a8.Lower(a9, points()));
}
