#include "../include/color.hpp"
#include "../include/interval.hpp"

inline double linear_to_gamma(double linear_comp) {
  if (linear_comp > 0)
    return std::sqrt(linear_comp);
  return 0;
}

void write_color(std::ostream &out, const color &pixel_color) {
  double r = pixel_color.x();
  double g = pixel_color.y();
  double b = pixel_color.z();

  if (r != r)
    r = 0;
  if (g != g)
    g = 0;
  if (b != b)
    b = 0;

  r = linear_to_gamma(r);
  g = linear_to_gamma(g);
  b = linear_to_gamma(b);

  static const interval intensity(0.000000, 0.9999999);
  int rbyte = int(256 * intensity.clamp(r));
  int gbyte = int(256 * intensity.clamp(g));
  int bbyte = int(256 * intensity.clamp(b));

  out << rbyte << ' ' << gbyte << ' ' << bbyte << '\n';
}
