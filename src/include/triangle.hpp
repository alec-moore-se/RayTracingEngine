#pragma once

#include "commons.hpp"
#include "hittable.hpp"
#include "material.hpp"
#include "vec3.hpp"

class triangle : public hittable {
  vec3 a, b, c;
  shared_ptr<material> mat;

public:
  triangle(vec3 a, vec3 b, vec3 c, shared_ptr<material> m)
      : a(a), b(b), c(c), mat(m) {}

  triangle(const point3 &p, double size, shared_ptr<material> m) {
    vec3 p1 = vec3(-size / 2.0, -sqrt(3) * size / 6.0, 0);
    vec3 p2 = vec3(size / 2.0, -sqrt(3) * size / 6.0, 0);
    vec3 p3 = vec3(0, sqrt(3) * size / 3.0, 0);
    a = p1 + p;
    b = p2 + p;
    c = p3 + p;
    mat = m;
  }

  bool hit(const ray &r, interval ray_t, hit_rec &rec) const override {
    return intersect_without_backface(r, ray_t, rec);
  }

  AABB bounding_box() const override {
    AABB aabb;
    return aabb;
  }

  bool intersect_without_backface(const ray &r, interval ray_t,
                                  hit_rec &rec) const {
    (void)ray_t;
    vec3 E1, E2, t, p, q;
    double det, i_det;
    E1 = (b - a);
    E2 = (c - a);
    p = cross_product(r.direction(), E2);
    det = dot_product(E1, p);
    if (det < epslion_flo)
      return false;

    t = (r.origin() - a);

    rec.u = dot_product(t, p);
    if (rec.u < 0.0 || rec.u > det)
      return false;

    q = cross_product(t, E1);

    rec.v = dot_product(r.direction(), q);
    if (rec.v < 0.0 || rec.u + rec.v > det) {
      return false;
    }

    rec.t = dot_product(E2, q);
    i_det = 1.0 / det;
    rec.t *= i_det;
    rec.u *= i_det;
    rec.v *= i_det;
    rec.set_face_normal(r, cross_product(E1, E2));
    rec.mat = mat;

    return true;
  }

  bool intersect_with_backface(const ray &r, interval ray_t,
                               hit_rec &rec) const {
    (void)ray_t;
    vec3 E1, E2, p, t, q;
    double det, i_det;
    if (det > -epslion_flo && det < epslion_flo) {
      return false;
    }
    i_det = 1.0 / det;

    t = r.origin() - a;
    rec.u = dot_product(t, p) * i_det;
    if (rec.u < 0.0 || rec.u > 1.0)
      return false;
    q = cross_product(t, E1);
    rec.v = dot_product(r.direction(), q) * i_det;
    if (rec.v < 0.0 || rec.v > 1.0)
      return false;
    rec.t = dot_product(E2, q) * i_det;

    return true;
  }

private:
};
