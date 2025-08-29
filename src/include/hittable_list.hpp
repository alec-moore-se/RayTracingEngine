#pragma once

#include "commons.hpp"
#include "hittable.hpp"


using std::make_shared;
using std::shared_ptr;
using std::vector;

struct hittable_list : public hittable {
  vector<shared_ptr<hittable>> objects;

  hittable_list() {}
  hittable_list(shared_ptr<hittable> object) { add(object); }

  void clear() { objects.clear(); }
  void add(shared_ptr<hittable> object) { objects.push_back(object); }

  bool hit(const ray &r, double ray_tmin, double ray_tmax,
           hit_rec &rec) const override {
    hit_rec tempRec;
    bool hit_ = false;
    auto closest_cur = ray_tmax;

    for (const auto &object : objects) {
      if (object->hit(r, ray_tmin, ray_tmax, tempRec)) {
        hit_ = true;
        closest_cur = tempRec.t;
        rec = tempRec;
      }
    }
    return hit_;
  }
};
