#pragma once
#include "interval.hpp"
#include "ray.hpp"
#include "vec3.hpp"
#include <cstdint>
#include <sys/types.h>

class AABB {

public:
  interval x, y, z;
  AABB() {}
  AABB(const interval &x, const interval &y, const interval &z)
      : x(x), y(y), z(z) {}
  AABB(const point3 &a, const point3 &b)
      : x((a.x() <= b.x()) ? interval(a.x(), b.x()) : interval(b.x(), a.x())),
        y((a.y() <= b.y()) ? interval(a.y(), b.y()) : interval(b.y(), a.y())),
        z((a.z() <= b.z()) ? interval(a.z(), b.z()) : interval(b.z(), a.z())) {}
  AABB(const AABB &b1, const AABB &b2) {
    x = interval(b1.x, b2.x);
    y = interval(b1.y, b2.y);
    z = interval(b1.z, b2.z);
  }

  const interval &get_axis(const uint8_t i) const {
    if (i == 1)
      return y;
    if (i == 2)
      return z;
    return x;
  }

  uint8_t longest_axis() const {
    if (x.size() > y.size())
      return x.size() > z.size() ? 0 : 2;
    else
      return y.size() > z.size() ? 1 : 2;
  }

  bool hit(const ray &r, interval ray_t) const {
    auto ray_orig = r.origin();
    auto ray_dir = r.direction();

    for (uint8_t i = 0; i < 3; i++) {
      auto ax = get_axis(i);
      // axis direction v
      double ad_inv = 1.0 / ray_dir[i];
      // at i?
      auto t0 = (ax.min - ray_orig[i]) * ad_inv;
      auto t1 = (ax.max - ray_orig[i]) * ad_inv;

      if (t0 < t1) {
        if (t0 > ray_t.min)
          ray_t.min = t0;
        if (t1 < ray_t.max)
          ray_t.max = t1;
      } else {
        if (t1 > ray_t.min)
          ray_t.min = t1;
        if (t0 < ray_t.max)
          ray_t.max = t0;
      }
      if (ray_t.max <= ray_t.min) {
        return false;
      }
    }
    return true;
  }

  static const AABB empty, universe;
};
