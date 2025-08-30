#pragma once
#include "commons.hpp"
#include "hittable.hpp"
#include "interval.hpp"

class sphere : public hittable {
  point3 center;
  double radius;

public:
  sphere(const point3 &center, const double radius)
      : center(center), radius(std::fmax(0, radius)) {}

  bool hit(const ray &r, interval ray_t, hit_rec &rec) const override {
    vec3 oc = center - r.origin();
    double a = dot_product(r.direction(), r.direction());
    double h = dot_product(r.direction(), oc);
    double c = dot_product(oc, oc) - (radius * radius);
    double discriminant = h * h - a * c;

    if (discriminant < 0)
      return false;
    auto sqrtd = (h - std::sqrt(discriminant)) / a;

    auto root = (h - sqrtd) / a;
    if (!ray_t.surrounds(root)) {
      root = (h + sqrtd) / a;
      if (!ray_t.surrounds(root))
        return false;
    }
    rec.t = root;
    rec.p = r.at(rec.t);
    // FIXME
    // vec3 outward_normal = unit_vector((rec.p - center) / radius);
    vec3 outward_normal = (rec.p - center) / radius;
    rec.set_face_normal(r, outward_normal);

    return true;
  }
};

double hit_sphere(const point3 &origin, double radius, const ray &r);
