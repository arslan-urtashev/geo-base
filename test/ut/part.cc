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
