#include "aabb.hpp"
#include "commons.hpp"
#include "hittable.hpp"
#include "material.hpp"
#include "vec3.hpp"
#include <memory>

class quadrilateral : public hittable {

  inline virtual void set_bounding_box() {
    AABB bbox_diagonal1 = AABB(corner, corner + u + v);
    AABB bbox_diagonal2 = AABB(corner + u, corner + v);
    aabb = AABB(bbox_diagonal1, bbox_diagonal2);
  }

public:
  quadrilateral(const point3 &p, const vec3 &u, const vec3 &v,
                shared_ptr<material> m)
      : corner(p), u(u), v(v), mat(m) {
    vec3 n = cross_product(u, v);
    normal = unit_vector(n);
    D = dot_product(normal, corner);
    w = n / dot_product(n, n);
    set_bounding_box();
  }
  quadrilateral(const quadrilateral &&q)
      : aabb(q.aabb), corner(q.corner), u(q.u), v(q.v), normal(q.normal),
        w(q.w), mat(q.mat), D(q.D) {}

  virtual bool hit(const ray &r, interval ray_t, hit_rec &rec) const override {

    double d = dot_product(r.direction(), normal);

    if (std::fabs(d) < epsilon_dou)
      return false;

    double t = (D - dot_product(normal, r.origin())) / d;
    if (!ray_t.contains(t))
      return false;

    auto intersection = r.at(t);

    auto corner_to_hit = intersection - corner;
    auto alpha = dot_product(w, cross_product(corner_to_hit, v));
    auto beta = dot_product(w, cross_product(u, corner_to_hit));

    if (!is_inside(alpha, beta, rec)) {
      return false;
    }
    rec.t = t;
    rec.p = intersection;
    rec.mat = mat;
    rec.set_face_normal(r, normal);

    return true;

    return true;
  }
  virtual AABB bounding_box() const override { return aabb; }
  virtual bool is_inside(double a, double b, hit_rec &rec) const {
    interval unit_interval(0, 1);
    if (!unit_interval.contains(a) || !unit_interval.contains(b))
      return false;

    rec.u = a;
    rec.v = b;

    return true;
  }

private:
  AABB aabb;
  point3 corner;
  vec3 u, v, normal, w;
  shared_ptr<material> mat;
  double D;
};
