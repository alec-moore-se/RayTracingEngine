#include "aabb.hpp"
#include "commons.hpp"
#include "hittable.hpp"
#include "hittable_list.hpp"
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

inline shared_ptr<hittable_list> box(const point3 &a, const point3 &b,
                                     shared_ptr<material> mat) {
  shared_ptr<hittable_list> sides = make_shared<hittable_list>();
  point3 min =
      point3(fmin(a.x(), b.x()), fmin(a.y(), b.y()), fmin(a.z(), b.z()));
  point3 max =
      point3(fmax(a.x(), b.x()), fmax(a.y(), b.y()), fmax(a.z(), b.z()));

  vec3 dx = vec3(max.x() - min.x(), 0, 0);
  vec3 dy = vec3(0, max.y() - min.y(), 0);
  vec3 dz = vec3(0, 0, max.z() - min.z());

  sides->add(make_shared<quadrilateral>(point3(min.x(), min.y(), max.z()), dx,
                                        dy, mat)); // front
  sides->add(make_shared<quadrilateral>(point3(max.x(), min.y(), max.z()), -dz,
                                        dy, mat)); // right
  sides->add(make_shared<quadrilateral>(point3(max.x(), min.y(), min.z()), -dx,
                                        dy, mat)); // back
  sides->add(make_shared<quadrilateral>(point3(min.x(), min.y(), min.z()), dz,
                                        dy, mat)); // left
  sides->add(make_shared<quadrilateral>(point3(min.x(), max.y(), max.z()), dx,
                                        -dz, mat)); // top
  sides->add(make_shared<quadrilateral>(point3(min.x(), min.y(), min.z()), dx,
                                        dz, mat)); // bottom

  return sides;
}
