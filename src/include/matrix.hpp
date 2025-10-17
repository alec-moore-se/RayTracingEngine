#include "commons.hpp"
#include "vec3.hpp"

// double array for general use case
class square_double_matrix {
  size_t length;
  vector<double> elems;
};

// for vec3 with a c/w
class vec4 {
  vec3 v;
  double w;

public:
  vec4() : v(vec3()), w(0) {}
  vec4(const vec3 &v, double w) : v(v), w(w) {}
};

// for the scene graph
class view_matrix {
  vec4 x, y, z, c;

public:
  view_matrix() : x(vec4()), y(vec4()), z(vec4()), c(vec4()) {}
};

/*
 * Notes:
 * matrix stack (hardcoded in lecture)
 * int stack_base
 * some form of init function??
 *
 * matrix_stack[0][0][0] = 1.0
 * matrix_stack[0][1][1] = 1.0
 * matrix_stack[0][2][2] = 1.0
 * matrix_stack[0][3][3] = 1.0
 *
 * void matrix_stack_pop() {
 *  if(matrix_stack_base)
 *    matrix_stack_base--;
 * }
 *
 *  void matrix_stack_rotz()
 *  {
 *    mult
 *    reulst in next level of a stack
 *  }
 *
 */
