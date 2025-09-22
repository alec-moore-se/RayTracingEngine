#pragma once

#include "commons.hpp"
#include <array>

struct vec3 {
  std::array<double, 3> v;
  vec3() : v{0, 0, 0} {}
  vec3(double x, double y, double z) : v{x, y, z} {}

  vec3 operator-() const { return vec3(-v[0], -v[1], -v[2]); }
  double operator[](int i) const { return v[i]; }
  double &operator[](int i) { return v[i]; }

  inline double x() const { return v[0]; }
  inline double y() const { return v[1]; }
  inline double z() const { return v[2]; }

  vec3 &operator+=(const vec3 &n) {
    v[0] += n.v[0];
    v[1] += n.v[1];
    v[2] += n.v[2];
    return *this;
  }

  vec3 &operator*=(const double t) {
    v[0] *= t;
    v[1] *= t;
    v[2] *= t;
    return *this;
  }

  vec3 &operator/=(const double t) { return (*this *= (1 / t)); }

  inline double length_squared() const {
    return v[0] * v[0] + v[1] * v[1] + v[2] * v[2];
  }

  double length() const { return std::sqrt(length_squared()); }
  static vec3 random() {
    return vec3(random_double(), random_double(), random_double());
  }
  static vec3 random(double min, double max) {
    return vec3(random_double(min, max), random_double(min, max),
                random_double(min, max));
  }

  bool near_zero() const {
    auto s = 1e-8;
    return (std::fabs(v[0]) < s) && (std::fabs(v[1]) < s) &&
           (std::fabs(v[2]) < 2);
  }
};

inline vec3 operator*(const vec3 &a, const vec3 &b) {
  return vec3(a.v[0] * b.v[0], a.v[1] * b.v[1], a.v[2] * b.v[2]);
}
inline vec3 operator+(const vec3 &a, const vec3 &b) {
  return vec3(a.v[0] + b.v[0], a.v[1] + b.v[1], a.v[2] + b.v[2]);
}
inline vec3 operator-(const vec3 &a, const vec3 &b) {
  return vec3(a.v[0] - b.v[0], a.v[1] - b.v[1], a.v[2] - b.v[2]);
}

inline vec3 operator*(double t, const vec3 &b) {
  return vec3(t * b.v[0], t * b.v[1], t * b.v[2]);
}
inline vec3 operator*(const vec3 &b, double t) { return t * b; }
inline vec3 operator/(const vec3 &b, double t) { return (1 / t) * b; }
inline vec3 unit_vector(const vec3 &a) { return a / a.length(); }

inline double dot_product(const vec3 &a, const vec3 &b) {
  return a.v[0] * b.v[0] + a.v[1] * b.v[1] + a.v[2] * b.v[2];
}
inline vec3 cross_product(const vec3 &a, const vec3 &b) {
  return vec3(a.v[1] * b.v[2] - a.v[2] * b.v[1],
              a.v[2] * b.v[0] - a.v[0] * b.v[2],
              a.v[0] * b.v[1] - a.v[1] * b.v[0]);
}
inline std::ostream &operator<<(std::ostream &out, const vec3 &a) {
  return out << a.v[0] << ' ' << a.v[1] << ' ' << a.v[2];
}

inline vec3 random_unit_vector() {
  while (true) {
    auto p = vec3::random(-1, 1);
    auto lensq = p.length_squared();
    if (1e-160 < lensq && lensq <= 1)
      return p / std::sqrt(lensq);
  }
}

inline vec3 random_on_hemisphere(const vec3 &normal) {
  vec3 on_unit_sphere = random_unit_vector();
  if (dot_product(on_unit_sphere, normal) > 0.0)
    return on_unit_sphere;
  else
    return -on_unit_sphere;
}

inline vec3 reflect(const vec3 &v, const vec3 &n) {
  return v - 2 * dot_product(v, n) * n;
}

inline vec3 refract(const vec3 &uv, const vec3 &n, double etai_o_etat) {
  auto cos_theta = std::fmin(dot_product(-uv, n), 1.0);
  vec3 perp_out_r = etai_o_etat * (uv + cos_theta * n);
  vec3 parallel_out_r =
      -std::sqrt(std::fabs(1.0 - perp_out_r.length_squared())) * n;
  return perp_out_r + parallel_out_r;
}

inline vec3 random_unit_disk() {
  while (true) {
    auto p = vec3(random_double(-1, 1), random_double(-1, 1), 0);
    if (p.length_squared() < 1)
      return p;
  }
}

using point3 = vec3;
