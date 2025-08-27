#include "../include/sphere.hpp"

double hit_sphere(const point3 &center, double radius, const ray &r) {
  vec3 oc = center - r.origin();
  double a = dot_product(r.direction(), r.direction());
  double h = dot_product(r.direction(), oc);
  double c = dot_product(oc, oc) - (radius * radius);
  double discriminant = h * h - a * c;

  if (discriminant < 0)
    return -1.0;
  else
    return (h - std::sqrt(discriminant)) / a;
}
