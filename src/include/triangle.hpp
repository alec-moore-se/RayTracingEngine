#pragma once

#include "commons.hpp"
#include "hittable.hpp"
#include "material.hpp"
#include "vec3.hpp"

class triangle : public hittable {
  vec3 a, b, c;
  ray center;
  vec3 n;
  shared_ptr<material> mat;

  double area() { return areaTot() / 2; }

public:
  triangle(vec3 a, vec3 b, vec3 c, shared_ptr<material> m)
      : a(a), b(b), c(c), center(ray()), n(N(*this)), mat(m) {}

  bool hit(const ray &r, interval ray_t, hit_rec &rec) const override {
    vec3 E1, E2, t, p, q;
    double det, i_det;
    E1 = (b - a);
    E2 = (c - a);
    p = cross_product(r.direction(), E2);
    det = dot_product(E1, p);
    if (det < epslion_flo) {
      return false;
    }

    t = (r.origin() - a);

    rec.u = dot_product(t, p);
    if (rec.u < 0.0 || rec.u > det) {
      return false;
    }

    q = cross_product(t, E1);

    rec.v = dot_product(r.direction(), q);
    if (rec.v < 0.0 || rec.v > det) {
      return false;
    }

    rec.t = dot_product(E2, q);
    i_det = 1.0 / det;
    rec.t *= i_det;
    rec.u *= i_det;
    rec.v *= i_det;
    rec.set_face_normal(r, p);
    rec.mat = mat;

    return true;
  }

  //  bool intersect_with_backface(const ray &r, interval ray_t, hit_rec &rec,
  //                               double &det, double &i_det, vec3 &E1, vec3
  //                               &E2, vec3 &t, vec3 &p, vec3 &q) {
  //    if (det > -epslion_flo && det < epslion_flo) {
  //      return false;
  //    }
  //    i_det = 1.0 / det;
  //
  //    t = r.origin() - a;
  //    rec.u = dot_product(t, p) * i_det;
  //    if (rec.u < 0.0 || rec.u > 1.0)
  //      return false;
  //    q = cross_product(t, E1);
  //    rec.v = dot_product(r.direction(), q) * i_det;
  //    if (rec.v < 0.0 || rec.v > 1.0)
  //      return false;
  //    rec.t = dot_product(E2, q) * i_det;
  //
  //    return true;
  //  }
  //  vec3 barycentric_coords(const point3 &P) {
  //    vec3 bary;
  //    bary[0] = (dot_product(N(*this), Na(*this, P))) / (areaTot() *
  //    areaTot()); bary[1] = (dot_product(N(*this), Nb(*this, P))) / (areaTot()
  //    * areaTot()); bary[2] = (dot_product(N(*this), Nc(*this, P))) /
  //    (areaTot() * areaTot()); return bary;
  //  }

  bool backface(const point3 &P) {
    auto Na_t = Na(*this, P);
    if (Na_t.length() < 0)
      return true;
    auto Nb_t = Nb(*this, P);
    if (Nb_t.length() < 0)
      return true;
    auto Nc_t = Nc(*this, P);
    if (Nc_t.length() < 0)
      return true;
    return false;
  }

private:
  inline vec3 Na(const triangle &T, const point3 &P) {
    return cross_product((T.c - T.b), (P - T.b));
  }
  inline vec3 Nb(const triangle &T, const point3 &P) {
    return cross_product((T.a - T.c), (P - T.c));
  }
  inline vec3 Nc(const triangle &T, const point3 &P) {
    return cross_product((T.b - T.a), (P - T.a));
  }
  inline double areaTot() { return (cross_product((b - a), (c - a))).length(); }
  inline vec3 N(const triangle &T) {
    return cross_product((T.b - T.a), (T.c - T.a));
  }
};
