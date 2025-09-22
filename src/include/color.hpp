#pragma once

#include "vec3.hpp"

using color = vec3;
void write_color(std::ostream &out, const color &pixel_color);
inline double linear_to_gamma(double linear_comp);
