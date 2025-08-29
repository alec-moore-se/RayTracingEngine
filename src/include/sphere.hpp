#pragma once
#include "commons.hpp"
#include "hittable.hpp"

class sphere : public hittable {
  point3 center;
  double radius;

public:
  sphere(const point3 &center, const double radius)
      : center(center), radius(std::fmax(0, radius)) {}

  bool hit(const ray &r, double ray_tmin, double ray_tmax,
           hit_rec &rec) const override {
    vec3 oc = center - r.origin();
    double a = dot_product(r.direction(), r.direction());
    double h = dot_product(r.direction(), oc);
    double c = dot_product(oc, oc) - (radius * radius);
    double discriminant = h * h - a * c;

    if (discriminant < 0)
      return false;
    auto sqrtd = (h - std::sqrt(discriminant)) / a;

    auto root = (h - sqrtd) / a;
    if (root >= ray_tmax || root <= ray_tmin) {
      root = (h + sqrtd) / a;
      if (root >= ray_tmax || root <= ray_tmin)
        return false;
    }
    rec.t = root;
    rec.p = r.at(rec.t);
    auto outward_normal = (rec.p - center) / radius;
    rec.set_face_normal(r, outward_normal);

    return true;
  }
};

double hit_sphere(const point3 &origin, double radius, const ray &r);
