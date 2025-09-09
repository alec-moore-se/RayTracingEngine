#pragma once

#include "color.hpp"
#include "commons.hpp"
#include "vec3.hpp"

class ray {
  point3 orig;
  vec3 dir;
  double time_;

public:
  ray() {}
  ray(const point3 &origin, const vec3 &direction, double t)
      : orig(origin), dir(direction), time_(t) {}

  ray(const point3 &origin, const vec3 &direction)
      : ray(origin, direction, 0) {}

  const point3 &origin() const { return orig; }
  const vec3 &direction() const { return dir; }
  const double time() const { return time_; }

  point3 at(double t) const { return orig + (t * dir); };
};
