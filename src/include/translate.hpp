#pragma once
#include "commons.hpp"
#include "hittable.hpp"
#include "interval.hpp"
#include "quaternian.hpp"
#include "ray.hpp"
#include "vec3.hpp"

class translate : public hittable {
  shared_ptr<hittable> ptr;
  vec3 offset;
  AABB bbox;

public:
  translate(shared_ptr<hittable> p, const vec3 &displacement)
      : ptr(p), offset(displacement) {
    bbox = AABB(p->bounding_box() + offset);
  }
  translate(shared_ptr<hittable> p, double displacement)
      : ptr(p), offset(displacement, displacement, displacement) {

    bbox = AABB(p->bounding_box() + offset);
  }

  bool hit(const ray &r, interval ray_t, hit_rec &rec) const override {
    ray moved_r(r.origin() - offset, r.direction(), r.time());
    if (!ptr->hit(moved_r, ray_t, rec))
      return false;
    rec.p += offset;
    return true;
  }

  AABB bounding_box() const override { return bbox; }
};

class rotate : public hittable {
  shared_ptr<hittable> obj;
  vec3 axis;
  double angle;
  AABB bbox;

public:
  rotate(const shared_ptr<hittable> &obj, const vec3 &axis, double theta)
      : obj(obj), axis(axis), angle(theta) {
    bbox = obj->bounding_box().rotateBB(axis, theta);
  }

  bool hit(const ray &r, interval ray_t, hit_rec &rec) const override {
    auto rotated_origin = Q::vec_rot(axis, -angle, r.origin());
    auto rotated_direction = Q::vec_rot(axis, -angle, r.direction());

    auto rotated_r = ray(rotated_origin, rotated_direction, r.time());
    if (!obj->hit(rotated_r, ray_t, rec))
      return false;

    rec.p = Q::vec_rot(axis, angle, rec.p);
    rec.set_face_normal(r, Q::vec_rot(axis, angle, rec.norm));
    return true;
  }

  AABB bounding_box() const override { return bbox; }
};
