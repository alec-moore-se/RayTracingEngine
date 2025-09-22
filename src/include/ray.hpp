#pragma once

#include "vec3.hpp"

class ray {
  point3 orig = point3(0, 0, 0);
  vec3 dir = vec3(0, 0, 0);
  double time_ = 0.0;

public:
  ray() {}
  ray(const point3 &origin, const vec3 &direction, double t)
      : orig(origin), dir(direction), time_(t) {}

  ray(const point3 &origin, const vec3 &direction)
      : ray(origin, direction, 0) {}

  const point3 &origin() const { return orig; }
  const vec3 &direction() const { return dir; }
  double time() const { return time_; }

  point3 at(double t) const { return orig + (t * dir); }
};
