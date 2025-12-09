#pragma once

#include <cmath>
#include <cstdint>
#include <future>
#include <iostream>
#include <memory>
#include <ostream>
#include <random>
#include <sstream>
#include <thread>
#include <vector>

#define epsilon_flo 1e-5
#define epsilon_dou 1e-15

using std::cos;
using std::make_shared;
using std::make_unique;
using std::shared_ptr;
using std::sin;
using std::tan;
using std::unique_ptr;
using std::vector;

#define infinity double(std::numeric_limits<double>::infinity())
#define PI 3.1415926535897932385

inline double deg_to_rad(double degrees) { return degrees * PI / 180.0; }

inline double random_double() {
  static std::uniform_real_distribution<double> distribution(0.0, 1.0);
  static std::mt19937_64 generator;
  return distribution(generator);
}
#define abs(x) (((x < 0) ? (-x) : (x)))
#define cmp_float(x, y) (abs(x - y) <= epsilon_flo)
#define cmp_double(x, y) (abs(x - y) <= epsilon_flo)

inline double random_double(double min, double max) {
  return min + (max - min) * random_double();
}

inline int random_int() { return int(random_double() + 1); }
inline int random_int(double min, double max) {
  return int(random_double(min, max + 1));
}
inline static int clamp(int x, int l, int h) {
  return (x < l) ? l : (x > h) ? h - 1 : x;
}
inline static double clamp(double x, double l, double h) {
  return (x < l) ? l : (x > h) ? h - 1 : x;
}
