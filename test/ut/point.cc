#include <gmock/gmock.h>

#include "point.h"

using namespace geo_base;

TEST(PointTest, CrossProduct) {
  Point a(5, 1), b(1, -1);
  EXPECT_TRUE(a.CrossProduct(b) < 0);
  EXPECT_TRUE(b.CrossProduct(a) > 0);
}
