#include "aabb.hpp"
#include "commons.hpp"
#include "material.hpp"
#include "vec3.hpp"
#include <memory>

class quadrilaterals : public hittable {
  using Q = quadrilaterals;

  inline virtual void set_bounding_box() {
    AABB bbox_diagonal1 = AABB(corner, corner + u + v);
    AABB bbox_diagonal2 = AABB(corner + u, corner + v);
    aabb = AABB(bbox_diagonal1, bbox_diagonal2);
  }

public:
  quadrilaterals(const point3 &p, const vec3 &u, const vec3 &v,
                 std::shared_ptr<material> m)
      : corner(p), u(u), v(v), mat(m) {
    normal = unit_vector(cross_product(u, v));
    set_bounding_box();
  }
  virtual bool hit(const ray &r, interval ray_t, hit_rec &rec) const override {
    // do stuff
    return true;
  }
  virtual AABB bounding_box() const override { return aabb; }

private:
  AABB aabb;
  point3 corner;
  vec3 u, v, normal;
  shared_ptr<material> mat;
  double D;
};
