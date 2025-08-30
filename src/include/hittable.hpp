#pragma once
#include "commons.hpp"
#include "interval.hpp"
#include "ray.hpp"
#include "vec3.hpp"

struct hit_rec {
  point3 p;
  vec3 norm;
  double t = 0.0;
  bool front_face = false;

  void set_face_normal(const ray &r, const vec3 &outward_normal) {
    front_face = dot_product(r.direction(), outward_normal) < 0;
    norm = front_face ? outward_normal : -outward_normal;
  }
};

struct hittable {
  virtual ~hittable() = default;
  virtual bool hit(const ray &r, interval ray_t, hit_rec &rec) const = 0;
};
