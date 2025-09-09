#pragma once
#include "commons.hpp"
#include "hittable.hpp"
#include "interval.hpp"
#include <memory>

class sphere : public hittable {
  ray center;
  double radius;
  std::shared_ptr<material> mat;

public:
  sphere(const point3 &static_center, double radius, shared_ptr<material> mat)
      : center(static_center, vec3()), radius(std::fmax(0, radius)), mat(mat) {}

  sphere(const point3 &center1, const point3 &center2, double radius,
         shared_ptr<material> mat)
      : center(center1, center2 - center1), radius(std::fmax(0, radius)),
        mat(mat) {}

  bool hit(const ray &r, interval ray_t, hit_rec &rec) const override {
    point3 current_center = r.at(r.time());
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
};
