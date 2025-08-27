#pragma once
#include "ray.hpp"
#include "vec3.hpp"

class sphere {};

double hit_sphere(const point3 &origin, double radius, const ray &r);
