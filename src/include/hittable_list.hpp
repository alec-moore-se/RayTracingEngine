#pragma once

#include "commons.hpp"
#include "hittable.hpp"
#include "interval.hpp"

using std::make_shared;
using std::shared_ptr;
using std::vector;

struct hittable_list : public hittable {
  vector<shared_ptr<hittable>> objects;

  hittable_list() {}
  hittable_list(shared_ptr<hittable> object) { add(object); }

  void clear() { objects.clear(); }
  void add(const shared_ptr<hittable> &object) {
    objects.push_back(object);
    aabb = AABB(aabb, object->bounding_box());
  }

  bool hit(const ray &r, interval ray_t, hit_rec &rec) const override {
    hit_rec tempRec;
    bool hit_ = false;
    auto closest_cur = ray_t.max;

    for (const auto &object : objects) {
      if (object->hit(r, interval(ray_t.min, closest_cur), tempRec)) {
        hit_ = true;
        closest_cur = tempRec.t;
        rec = tempRec;
      }
    }
    return hit_;
  }
  AABB bounding_box() const override { return aabb; }

  double pdf_value(const point3 &o, const vec3 &v) const override {
    auto weight = 1.0 / objects.size();
    auto sum = 0.0;

    for (const auto &object : objects) {
      sum += weight * object->pdf_value(o, v);
    }
    return sum;
  }

  vec3 random(const point3 &o) const override {
    int size = int(objects.size());
    return objects[random_int(0, size - 1)]->random(o);
  }

private:
  AABB aabb;
};
