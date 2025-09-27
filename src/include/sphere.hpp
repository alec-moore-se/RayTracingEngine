#pragma once
#include "commons.hpp"
#include "hittable.hpp"
#include "interval.hpp"
#include <memory>

class sphere : public hittable {
  AABB aabb;
  ray center;
  double radius;
  std::shared_ptr<material> mat;

public:
  sphere(const point3 &static_center, double radius, shared_ptr<material> mat)
      : center(static_center, vec3(0, 0, 0)), radius(std::fmax(0, radius)),
        mat(mat) {
    auto rvec = vec3(radius, radius, radius);
    aabb = AABB(static_center - rvec, static_center + rvec);
  }

  sphere(const point3 &center1, const point3 &center2, double radius,
         shared_ptr<material> mat)
      : center(center1, center2 - center1), radius(std::fmax(0, radius)),
        mat(mat) {
    auto rvec = vec3(radius, radius, radius);
    auto box1 = AABB(center.at(0) - rvec, center.at(0) + rvec);
    auto box2 = AABB(center.at(1) - rvec, center.at(1) + rvec);
    aabb = AABB(box1, box2);
  }

  bool hit(const ray &r, interval ray_t, hit_rec &rec) const override {
    point3 current_center = center.at(r.time());
    vec3 oc = current_center - r.origin();
    double a = r.direction().length_squared();
    double h = dot_product(r.direction(), oc);
    double c = oc.length_squared() - (radius * radius);
    double discriminant = ((h * h) - (a * c));

    if (discriminant < 0)
      return false;

    auto sqrtd = std::sqrt(discriminant);

    auto root = (h - sqrtd) / a;
    if (!ray_t.surrounds(root)) {
      root = (h + sqrtd) / a;
      if (!ray_t.surrounds(root))
        return false;
    }
    rec.t = root;
    rec.p = r.at(rec.t);
    // already normalized due to the nature of a sphere
    vec3 outward_normal = (rec.p - current_center) / radius;
    rec.set_face_normal(r, outward_normal);
    rec.mat = mat;

    return true;
  }
  AABB bounding_box() const override { return aabb; }
};
