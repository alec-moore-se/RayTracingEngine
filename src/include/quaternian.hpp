
#include "commons.hpp"
#include "vec3.hpp"

class quaternian {
  using Q = quaternian;
  // Hamilton i^2 = j^2 = k^2 = ijk = 1
  // Rules:
  // Multiplication Q(p) = <S_p, u_p, v_p, w_p> S is scalar, u,v,w are elems
  // or <S_p, V_p>
  // some p and q (Q) -> <S_p * S_q - dot(V_p, V_q), cross(V_p, V_q) + S_pV_q +
  // S_qV_p>
  // Inversion:
  // p^-1 = <S, -X, -Y, -Z>
  // Given a vector V make Q => p = Q(0, V)
  // rotation quaternian q = as above
  // The rotation p' of p around q is given by p' = q^{-1}pq
  // and to reverse by p = qp'q^-1
  // Axis-Angle <X, Y, Z> must be unit length
  // angle = theta
  // quaternian < cos theta / 2, x sin theta /2, repeat y, z>

  vec3 V;
  double S;

  inline Q &operator*=(const Q &q) {
    this->S = this->S * q.S - dot_product(this->V, q.V);
    this->V = cross_product(this->V, q.V) + this->S * q.V + q.S * this->V;
    return *this;
  }
  inline Q &operator*(const Q &q) {
    this->S = this->S * q.S - dot_product(this->V, q.V);
    this->V = cross_product(this->V, q.V) + this->S * q.V + q.S * this->V;
    return *this;
  }
  inline Q &operator-() {
    -this->V;
    return *this;
  }

  inline Q operator-(const Q &p) const { return Q(p.S, -p.V); }

  /*
   * Rotate and return new Quaternian, axis must be unit_length and angle is
   * assumed to be in degrees
   * @return Quaternian with rotation applied
   */
  inline Q quat_rot(vec3 axis, double angle) {
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
    p.S = 1;
    p.V = rot.V;

    // Rotation p_prime == qinv * p * q
    // Reversing p = q * p_prime * qinv
    return -rot * p * rot;
  }

  inline vec3 vec_rot(vec3 axis, double angle, vec3 point) {
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
    Q q_inv = -q;

    // no scalar, think like unit_length
    Q p;
    p.S = 1;
    p.V = point;

    // Rotation p_prime == qinv * p * q
    // Reversing p = q * p_prime * qinv
    Q res = q_inv * p * q;

    return res.V;
  }

public:
  quaternian() : S(0), V(vec3()) {}
  quaternian(double S, vec3 V) : S(S), V(V) {}
  quaternian(double S, double a, double b, double c) : S(S), V(vec3(a, b, c)) {}
  double scalar() const { return S; }
  vec3 vector() const { return V; }
};

using Q = quaternian;

inline Q operator*(const Q &p, const Q &q) {
  return Q(p.scalar() * q.scalar() - dot_product(p.vector(), q.vector()),
           cross_product(p.vector(), q.vector()) + p.scalar() * q.vector() +
               q.scalar() * p.vector());
}
