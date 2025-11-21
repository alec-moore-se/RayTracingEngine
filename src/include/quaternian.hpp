#pragma once

#include "commons.hpp"
#include "vec3.hpp"

#define rotX vec3(1, 0, 0)
#define rotY vec3(0, 1, 0)
#define rotZ vec3(0, 0, 1)

class quaternian {
  // I dont what to type all that
  using Q = quaternian;

  double S;
  vec3 V;

public:
  quaternian() : S(0), V(vec3()) {}
  quaternian(double S, vec3 V) : S(S), V(V) {}
  quaternian(double S, double a, double b, double c) : S(S), V(vec3(a, b, c)) {}
  double scalar() const { return S; }
  vec3 vector() const { return V; }

  inline Q &operator*=(const Q &q) {
    this->S = this->S * q.S - dot_product(this->V, q.V);
    this->V = cross_product(this->V, q.V) + this->S * q.V + q.S * this->V;
    return *this;
  }
  inline Q operator*(const Q &q) const {
    return Q(S * q.S - dot_product(V, q.V),
             cross_product(V, q.V) + S * q.V + q.S * V);
  }
  inline Q operator-() { return Q(this->S, -this->V); }

  inline Q operator-(const Q &p) const { return Q(p.S, -p.V); }

  /*
   * Rotate and return new Quaternian, axis must be unit_length and angle is
   * assumed to be in degrees
   * @return Quaternian with rotation applied
   */
  inline static Q quat_rot(vec3 axis, double angle) {
    // t = theta to rad
    // axis must be unit length
    // rot = <cos t/2, x sin t/2, y sin t/2, z sin t/2>
    double t = deg_to_rad(angle);
    double half = t / 2.0;
    double t_sin_ = sin(half);
    double t_cos_ = cos(half);
    Q p;
    Q rot;

    rot.S = t_cos_;
    rot.V = axis * t_sin_;

    // no scalar, think like unit_length
    p.S = 0;
    p.V = rot.V;

    // Rotation p_prime == qinv * p * q
    // Reversing p = q * p_prime * qinv
    Q res = (-rot) * p * rot;
    return res;
  }

  // rotation for a single vector in a plane
  inline static vec3 vec_rot(vec3 axis, double angle, vec3 point) {
    // t = theta to rad
    // axis must be unit length
    // rot = <cos t/2, x sin t/2, y sin t/2, z sin t/2>
    double t = deg_to_rad(angle);
    double half = t / 2.0;
    double t_sin_ = sin(half);
    double t_cos_ = cos(half);
    Q q;

    q.S = t_cos_;
    q.V = axis * t_sin_;

    // no scalar, think like unit_length
    Q p;
    p.S = 0;
    p.V = point;

    // Rotation p_prime == qinv * p * q
    // Reversing p = q * p_prime * qinv
    Q res = q * p * -q;

    // returning a copy for safety
    return vec3(res.V);
  }
};

using Q = quaternian;

inline Q operator*(const Q &p, const Q &q) {
  return Q(p.scalar() * q.scalar() - dot_product(p.vector(), q.vector()),
           cross_product(p.vector(), q.vector()) + p.scalar() * q.vector() +
               q.scalar() * p.vector());
}
