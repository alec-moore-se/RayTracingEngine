#pragma once
#include "aabb.hpp"
#include "interval.hpp"
#include "ray.hpp"
#include "vec3.hpp"
#include <memory>

class material;

struct hit_rec {
  point3 p;
  vec3 norm;
  std::shared_ptr<material> mat;
  double t = 0.0;          // time
  double u = 0.0, v = 0.0; // bary-centric || texture mapping
  bool front_face = false;

  void set_face_normal(const ray &r, const vec3 &outward_normal) {
    front_face = dot_product(r.direction(), outward_normal) < 0;
    norm = front_face ? outward_normal : -outward_normal;
  }
};

struct hittable {
  virtual ~hittable() = default;
  virtual bool hit(const ray &r, interval ray_t, hit_rec &rec) const = 0;
  virtual AABB bounding_box() const = 0;
  virtual double pdf_value(const point3 &o, const vec3 &v) const {
    (void)o;
    (void)v;
    return 0.00000;
  }
  virtual vec3 random(const point3 &o) const {
    (void)o;
    return vec3(1, 0, 0);
  }
};
