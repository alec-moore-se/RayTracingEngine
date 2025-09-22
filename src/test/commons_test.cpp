#include "tests.hpp"
#include <cassert>

void abs_test() {
  int x = -1;
  int y = 2;
  double z = -.33333333;
  double p = .000000000001;
  double h = -0.0000004;
  // checking for equality in floats usually a bad idea, however, I believe this
  // should be fine due to no arithmetic, only sign change, I believe the ALU
  // will have no effect on the equality of these vars.
  assert(abs(x) == 1);
  assert(abs(y) == 2);
  assert(abs(z) == .33333333);
  assert(abs(p) == .000000000001);
  assert(abs(h) == 0.0000004);
}

void compare_float_test() {
  double x = 1e-15;
  double y = -1000000000000;
  double p = -1e-4;
  assert(cmp_float((x - y), 0) == false);
  assert(cmp_float((p), 0) == false);
  assert(cmp_float((x), 0) == true);
}
void compare_double_test() {

  double x = 1e-15;
  double y = -1000000000000;
  double p = -1e-4;
  assert(cmp_double((x - y), 0) == false);
  assert(cmp_double((p), 0) == false);
  assert(cmp_double((x), 0) == true);
}
