#pragma once

#include "commons.hpp"
#include "hittable.hpp"
#include "vec3.hpp"

class triangle : public hittable {
  vec3 a, b, c;

  double area() { return (cross_product((b - a), (c - a)) / 2).length(); }

public:
  bool hit(const ray &r, interval ray_t, hit_rec &rec) const override {
    return true;
  }

  bool backface(const point3 &P) {
    // tmp line dont inline
    return false;
  }
};
