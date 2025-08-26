#include "../include/ray.hpp"
#include "../include/sphere.hpp"
color ray_color(const ray &r) {
  if (hit_sphere(point3(0, 0, -1), 0.5, r)) {
    return color(1, 0, 0);
  }
  vec3 unit_direction = unit_vector(r.direction());
  auto a = 0.5 * (unit_direction.y() + 1.0);
  return (1.0 - a) * color(1.0, 1.0, 1.0) + a * color(0.5, 0.7, 1.0);
}
