#pragma once

#include <cmath>
#include <cstdint>
#include <iostream>
#include <memory>
#include <ostream>
#include <vector>

using std::make_shared;
using std::make_unique;
using std::shared_ptr;
using std::unique_ptr;
using std::vector;

#define infinity double(std::numeric_limits<double>::infinity())
#define PI 3.1415926535897932385

inline double deg_to_rad(double degrees) { return degrees * PI / 180.0; }
