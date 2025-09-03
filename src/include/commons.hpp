#pragma once

#include <cmath>
#include <cstdint>
#include <iostream>
#include <memory>
#include <ostream>
#include <random>
#include <vector>

using std::make_shared;
using std::make_unique;
using std::shared_ptr;
using std::unique_ptr;
using std::vector;

const double infinity = double(std::numeric_limits<double>::infinity());
#define PI 3.1415926535897932385

inline double deg_to_rad(double degrees) { return degrees * PI / 180.0; }

inline double random_double() {
  static std::uniform_real_distribution<double> distribution(0.0, 1.0);
  static std::mt19937_64 generator;
  return distribution(generator);
}

inline double random_double(double min, double max) {
  return min + (max - min) * random_double();
}
