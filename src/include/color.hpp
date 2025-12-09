#pragma once

#include "commons.hpp"
#include "vec3.hpp"

using color = vec3;
void write_color(std::ostream &out, const color &pixel_color);
inline double linear_to_gamma(double linear_comp);

inline color clamp(color c, double min, double max) {
  return color(clamp(c.x(), min, max), clamp(c.y(), min, max),
               clamp(c.z(), min, max));
}

inline color clamp(color c, double max) { return clamp(c, 0.0, max); }
