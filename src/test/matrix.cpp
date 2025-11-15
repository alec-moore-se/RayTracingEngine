#include "../include/matrix.hpp"
#include <gtest/gtest.h>

TEST(Matrix, SquareDoubleMatrix) {
  square_double_matrix<3, double> m;
  m(0, 0) = 1;
  m(1, 1) = 2;
  m(2, 2) = 3;

  square_double_matrix<3, double> n;
  n(0, 0) = 4;
  n(1, 1) = 5;
  n(2, 2) = 6;

  EXPECT_EQ(m(0, 0), 1);
  EXPECT_EQ(m(1, 1), 2);
  EXPECT_EQ(m(2, 2), 3);

  m *= n;
  EXPECT_EQ(m(0, 0), 4);
  EXPECT_EQ(m(1, 1), 10);
  EXPECT_EQ(m(2, 2), 18);

  m *= 2;
  EXPECT_EQ(m(0, 0), 8);
  EXPECT_EQ(m(1, 1), 20);
  EXPECT_EQ(m(2, 2), 36);

  m += n;
  EXPECT_EQ(m(0, 0), 12);
  EXPECT_EQ(m(1, 1), 22);
  EXPECT_EQ(m(2, 2), 42);

  m = n;
  EXPECT_EQ(m(0, 0), 4);
  EXPECT_EQ(m(1, 1), 5);
}
