#include "../include/sphere.hpp"

bool hit_sphere(const point3 &center, double radius, const ray &r) {
  vec3 oc = center - r.origin();
  double a = dot_product(r.direction(), r.direction());
  double b = -2.0 * dot_product(r.direction(), oc);
  double c = dot_product(oc, oc) - (radius * radius);
  auto discriminant = b * b - 4 * a * c;
  return (discriminant >= 0);
}
