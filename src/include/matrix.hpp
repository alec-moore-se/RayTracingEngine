#pragma once
#include "commons.hpp"

template <size_t dim_row, size_t dim_col, typename T> class matrix {
  std::array<std::array<T, dim_col>, dim_row> data;

public:
  matrix() { data.fill(0); }
  matrix(const matrix &m) {
    for (size_t i = 0; i < dim_row; i++)
      for (size_t j = 0; j < dim_col; j++)
        data[i][j] = m(i, j);
  }
  matrix(const std::array<std::array<T, dim_col>, dim_row> &m) {
    for (size_t i = 0; i < dim_row; i++)
      for (size_t j = 0; j < dim_col; j++)
        data[i][j] = m[i][j];
  }

  const size_t get_dim_row() const { return dim_row; }
  const size_t get_dim_col() const { return dim_col; }

  T &operator()(size_t i, size_t j) { return data[i][j]; }
  const T &operator()(size_t i, size_t j) const { return data[i][j]; }
  std::array<T, dim_col> &get_row(size_t i) { return data[i]; }
  std::array<T, dim_row> &get_col(size_t i) {
    std::array<T, dim_row> temp;
    for (size_t j = 0; j < dim_row; j++)
      temp[j] = data[j][i];
    return temp[i];
  }

  inline matrix<dim_row, dim_col, T> &
  operator*=(const matrix<dim_row, dim_col, T> &m) {
    for (size_t i = 0; i < dim_row; i++)
      for (size_t j = 0; j < dim_col; j++)
        data[i][j] *= m(i, j);
    return *this;
  }

  inline matrix<dim_row, dim_col, T> &
  operator+=(const matrix<dim_row, dim_col, T> &m) {
    for (size_t i = 0; i < dim_row; i++)
      for (size_t j = 0; j < dim_col; j++)
        data[i][j] += m(i, j);
    return *this;
  }

  inline matrix<dim_row, dim_col, T> &
  operator-=(const matrix<dim_row, dim_col, T> &m) {
    for (size_t i = 0; i < dim_row; i++)
      for (size_t j = 0; j < dim_col; j++)
        data[i][j] -= m(i, j);
    return *this;
  }
  inline matrix<dim_row, dim_col, T> &
  operator/=(const matrix<dim_row, dim_col, T> &m) {
    for (size_t i = 0; i < dim_row; i++)
      for (size_t j = 0; j < dim_col; j++)
        if (m(i, j) != 0)
          data[i][j] /= m(i, j);
        else
          data[i][j] /= epsilon_dou;
    return *this;
  }

  inline matrix<dim_row, dim_col, T> &
  operator=(const matrix<dim_row, dim_col, T> &m) {
    for (size_t i = 0; i < dim_row; i++)
      for (size_t j = 0; j < dim_col; j++)
        data[i][j] = m(i, j);
    return *this;
  }

  inline void print_matrix() const {
    for (size_t i = 0; i < dim_row; i++) {
      for (size_t j = 0; j < dim_col; j++) {
        std::cout << '[' << data[i][j] << " ";
      }
      std::cout << ']' << std::endl;
    }
    std::cout << std::endl;
  }
};

template <size_t dim_row, size_t dim_col, typename T>
inline matrix<dim_row, dim_col, T>
operator-(const matrix<dim_row, dim_col, T> &m,
          const matrix<dim_row, dim_col, T> &n) {
  matrix<dim_row, dim_col, T> temp;
  for (size_t i = 0; i < dim_row; i++)
    for (size_t j = 0; j < dim_col; j++)
      temp[i][j] = m(i, j) - n(i, j);
  return temp;
}

template <size_t dim_row, size_t dim_col, typename T>
inline matrix<dim_row, dim_col, T>
operator+(const matrix<dim_row, dim_col, T> &m,
          const matrix<dim_row, dim_col, T> &n) {
  matrix<dim_row, dim_col, T> temp;
  for (size_t i = 0; i < dim_row; i++)
    for (size_t j = 0; j < dim_col; j++)
      temp[i][j] = m(i, j) + n(i, j);
  return temp;
}

template <size_t dim_row, size_t dim_col, typename T>
inline matrix<dim_row, dim_col, T>
operator*(const matrix<dim_row, dim_col, T> &m,
          const matrix<dim_row, dim_col, T> &n) {
  matrix<dim_row, dim_col, T> temp;
  for (size_t i = 0; i < dim_row; i++)
    for (size_t j = 0; j < dim_col; j++)
      temp[i][j] = m(i, j) * n(i, j);
  return temp;
}
