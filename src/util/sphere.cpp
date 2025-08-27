#include "../include/sphere.hpp"

double hit_sphere(const point3 &center, double radius, const ray &r) {
  vec3 oc = center - r.origin();
  double a = dot_product(r.direction(), r.direction());
  double b = -2.0 * dot_product(r.direction(), oc);
  double c = dot_product(oc, oc) - (radius * radius);
  double discriminant = b * b - 4 * a * c;

  if (discriminant < 0)
    return -1.0;
  else 
    return (-b - std::sqrt(discriminant)) / (2.0 * a);

}
