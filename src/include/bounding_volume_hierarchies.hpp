#pragma once
#include "commons.hpp"
#include "hittable.hpp"
#include "hittable_list.hpp"
#include <algorithm>
#include <cstdint>
class BVH : public hittable {
  AABB aabb;
  shared_ptr<hittable> left, right;

public:
  BVH(hittable_list &list) : BVH(list.objects, 0, list.objects.size()) {}
  BVH(vector<shared_ptr<hittable>> &list, size_t start, size_t end) {
    aabb = AABB::empty;
    for (size_t i = start; i < end; i++)
      aabb = AABB(aabb, list[i]->bounding_box());

    auto axis = aabb.longest_axis();

    auto comp = (axis == 0) ? xbox_comp : (axis == 1) ? ybox_comp : zbox_comp;

    size_t obj_size = end - start;

    if (obj_size == 1)
      left = right = list[start];
    else if (obj_size == 2) {
      left = list[start];
      right = list[start + 1];
    } else {
      std::sort(std::begin(list) + start, std::begin(list) + end, comp);
      size_t mid = start + obj_size / 2;
      left = make_shared<BVH>(list, start, mid);
      right = make_shared<BVH>(list, mid, end);
    }
    aabb = AABB(left->bounding_box(), right->bounding_box());
  }

  bool hit(const ray &r, interval ray_t, hit_rec &rec) const override {
    if (!aabb.hit(r, ray_t))
      return false;

    auto hit_left = left->hit(r, ray_t, rec);
    auto hit_right =
        right->hit(r, interval(ray_t.min, (hit_left) ? rec.t : ray_t.max), rec);

    return hit_left || hit_right;
  }

  AABB bounding_box() const override { return aabb; }

private:
  static bool box_compare(const shared_ptr<hittable> &a,
                          const shared_ptr<hittable> &b, uint8_t i) {
    auto a_axis = a->bounding_box().get_axis(i);
    auto b_axis = b->bounding_box().get_axis(i);
    return a_axis.min < b_axis.min;
  }

  static bool xbox_comp(const shared_ptr<hittable> &a,
                        const shared_ptr<hittable> &b) {
    return box_compare(a, b, 0);
  }
  static bool ybox_comp(const shared_ptr<hittable> &a,
                        const shared_ptr<hittable> &b) {
    return box_compare(a, b, 1);
  }
  static bool zbox_comp(const shared_ptr<hittable> &a,
                        const shared_ptr<hittable> &b) {
    return box_compare(a, b, 2);
  }
};
