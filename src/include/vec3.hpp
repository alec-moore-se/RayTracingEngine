#pragma once

#include <cmath>
#include <cstdint>
#include <ostream>
struct vec3 {
  double v[3];
  vec3() : v{0, 0, 0} {}
  vec3(double x, double y, double z) : v{x, y, z} {}

  vec3 operator-() const { return vec3(-v[0], -v[1], -v[2]); }
  double operator[](uint8_t i) const { return v[i]; }
  double &operator[](uint8_t i) { return v[i]; }

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

  // debug
  char p[100];
  std::string toString() const {
    std::sprintf((char *)p, "[%.5f, %.5f, %.5f]", v[0], v[1], v[2]);
    return p;
  }
};

inline vec3 operator*(const vec3 &a, const vec3 &b) {
  return vec3(a.x() * b.x(), a.y() * b.y(), a.z() * b.z());
}
inline vec3 operator+(const vec3 &a, const vec3 &b) {
  return vec3(a.x() + b.x(), a.y() + b.y(), a.z() + b.z());
}
inline vec3 operator-(const vec3 &a, const vec3 &b) {
  return vec3(a.x() - b.x(), a.y() - b.y(), a.z() - b.z());
}

inline vec3 operator*(double t, const vec3 &b) {
  return vec3(t * b.x(), t * b.y(), t * b.z());
}
inline vec3 operator*(const vec3 &b, double t) { return t * b; }
inline vec3 operator/(const vec3 &b, double t) { return (1 / t) * b; }
inline vec3 unit_vector(const vec3 &a) { return a / a.length(); }

inline double dot_product(const vec3 &a, const vec3 &b) {
  return a.x() * b.x() + a.y() * b.y() + a.z() * b.z();
}
inline vec3 cross_product(const vec3 &a, const vec3 &b) {
  return vec3(a.y() * b.z() - a.z() * b.y(), a.z() * b.x() - a.x() * b.z(),
              a.x() * b.y() - a.y() * b.x());
}
inline std::ostream &operator<<(std::ostream &out, const vec3 &a) {
  return out << a.x() << ' ' << a.y() << ' ' << a.z();
}

using point3 = vec3;
