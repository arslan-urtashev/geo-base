#include "geo_base_test.h"

using namespace geo_base;

class PartTest : public GeoBaseTest {
};

TEST_F(PartTest, TwoEdges) {
  PartHelper part = MakePart(0, {
    MakeEdge(Point(0, 0), Point(3, 0)),
    MakeEdge(Point(0, 3), Point(5, 5))   
  });

  EXPECT_EQ(0, part.coordinate);
  EXPECT_TRUE(part.Contains(Point(1, 1), geo_data()));
}

TEST_F(PartTest, PartContainsPoint) {
  PartHelper part = MakePart(0, {
    MakeEdge(Point(0, 0), Point(3, 0)),
    MakeEdge(Point(0, 3), Point(3, 0)),
    MakeEdge(Point(-1, 10), Point(10, 10)),
    MakeEdge(Point(-1, 10), Point(10, 100))
  });

  EXPECT_TRUE(part.Contains(Point(1, 1), geo_data()));
  EXPECT_TRUE(part.Contains(Point(3, 0), geo_data()));
  EXPECT_TRUE(part.Contains(Point(0, 11), geo_data()));
  EXPECT_TRUE(part.Contains(Point(0, 3), geo_data()));
  
  EXPECT_FALSE(part.Contains(Point(2, 5), geo_data()));
  EXPECT_FALSE(part.Contains(Point(2, -1), geo_data()));
  EXPECT_FALSE(part.Contains(Point(2, 101), geo_data()));
}
