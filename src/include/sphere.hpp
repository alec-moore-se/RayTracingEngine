#pragma once
#include "commons.hpp"
#include "hittable.hpp"
#include "importance_sampling.hpp"
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
    get_uv(outward_normal, rec.u, rec.v);
    rec.mat = mat;

    return true;
  }
  AABB bounding_box() const override { return aabb; }

  double pdf_value(const point3 &o, const vec3 &v) const override {
    hit_rec rec;
    if (!this->hit(ray(o, v), interval(0.0001, infinity), rec))
      return 0;

    auto distance_squared = (center.at(0) - o).length_squared();
    auto cosine_theta = std::sqrt(1 - radius * radius / distance_squared);
    auto solid_angle = 2 * PI * (1 - cosine_theta);

    return 1 / solid_angle;
  }

  vec3 random(const point3 &o) const override {
    auto direction = center.at(0) - o;
    auto distance_squared = direction.length_squared();
    auto onb = importance_sampling::orthonormal_basis(direction);
    return importance_sampling::transform_onb(
        rand_sphere(radius, distance_squared), onb);
  }

private:
  static void get_uv(const point3 &p, double &u, double &v) {
    auto theta = std::acos(-p.y());
    auto phi = std::atan2(p.z(), -p.x());

    u = phi / (2.0 * PI);
    v = theta / PI;
  }

  static vec3 rand_sphere(double radius, double distance_squared) {
    auto r1 = random_double();
    auto r2 = random_double();
    auto r3 = 1 + r2 * (std::sqrt(1 - radius * radius / distance_squared) - 1);

    auto phi = 2 * PI * r1;
    auto x = std::cos(phi) * std::sqrt(1 - r3 * r3);
    auto y = std::sin(phi) * std::sqrt(1 - r3 * r3);

    return vec3(x, y, r3);
  }
};
