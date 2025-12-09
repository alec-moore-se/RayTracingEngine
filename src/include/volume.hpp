#pragma once

#include "commons.hpp"
#include "material.hpp"
#include "texture.hpp"

class absorption : public hittable {
  shared_ptr<hittable> boundary;
  double negative_inverse;
  shared_ptr<material> phase_func;

public:
  absorption(shared_ptr<hittable> boundary, double density,
             shared_ptr<Texture> tex)
      : boundary(boundary), negative_inverse(-1 / density),
        phase_func(make_shared<isotropic>(tex)) {}
  absorption(shared_ptr<hittable> boundary, double density, const color &albedo)
      : boundary(boundary), negative_inverse(-1 / density),
        phase_func(make_shared<isotropic>(albedo)) {}
  bool hit(const ray &r, interval ray_t, hit_rec &rec) const override {
    hit_rec rec1, rec2;
    if (!boundary->hit(r, interval::universe, rec1))
      return false;
    if (!boundary->hit(r, interval(rec1.t + .0001, infinity), rec2))
      return false;

    if (rec1.t < ray_t.min)
      rec1.t = ray_t.min;
    if (rec2.t > ray_t.max)
      rec2.t = ray_t.max;

    if (rec1.t >= rec2.t)
      return false;

    if (rec1.t < 0)
      rec1.t = 0;

    auto ray_length = r.direction().length();
    auto distance_inside_boundary = (rec2.t - rec1.t) * ray_length;
    auto hit_distance = negative_inverse * std::log(random_double());

    if (hit_distance > distance_inside_boundary)
      return false;

    rec.t = rec1.t + hit_distance / ray_length;
    rec.p = r.at(rec.t);
    rec.norm = rec.p - rec1.p;
    rec.front_face = true;
    rec.mat = phase_func;
    return true;
  }

  AABB bounding_box() const override { return boundary->bounding_box(); }
};

class emission : public hittable {};
class scattering : public hittable {};
