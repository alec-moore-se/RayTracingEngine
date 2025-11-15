#include "commons.hpp"
#include "vec3.hpp"

// double array for general use case
template <size_t size, typename T> class square_double_matrix {
  std::array<T, size> data;

public:
  square_double_matrix() {
    for (size_t i = 0; i < size; i++)
      data[i] = 0;
  }

  T &operator()(size_t i, size_t j) { return data[i * size + j]; }

  const T &operator()(size_t i, size_t j) const { return data[i * size + j]; }

  square_double_matrix<size, T> &operator*=(const square_double_matrix &m) {
    for (size_t i = 0; i < size; i++)
      for (size_t j = 0; j < size; j++)
        data[i * size + j] *= m(i, j);
    return *this;
  }

  square_double_matrix<size, T> &
  operator*(const square_double_matrix &m) const {
    for (size_t i = 0; i < size; i++)
      for (size_t j = 0; j < size; j++)
        data[i * size + j] += m(i, j);
    return *this;
  }

  square_double_matrix<size, T> &operator+=(const square_double_matrix &m) {
    for (size_t i = 0; i < size; i++)
      for (size_t j = 0; j < size; j++)
        data[i * size + j] += m(i, j);
    return *this;
  }

  square_double_matrix<size, T> &operator*=(double t) {
    for (size_t i = 0; i < size; i++)
      for (size_t j = 0; j < size; j++)
        data[i * size + j] = data[i * size + j] * t;
    return *this;
  }

  square_double_matrix<size, T> &operator=(const square_double_matrix &m) {
    for (size_t i = 0; i < size; i++)
      for (size_t j = 0; j < size; j++)
        data[i * size + j] = m(i, j);
    return *this;
  }

  square_double_matrix<size, T> &operator-=(const square_double_matrix &m) {
    for (size_t i = 0; i < size; i++)
      for (size_t j = 0; j < size; j++)
        data[i * size + j] -= m(i, j);
    return *this;
  }
};

// for vec3 with a c/w
class vec4 {
  vec3 v;
  double w;

public:
  vec4() : v(vec3()), w(0) {}
  vec4(const vec3 &v, double w) : v(v), w(w) {}
  vec4 &operator*=(double t) {
    this->v *= t;
    this->w *= t;
    return *this;
  }
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
