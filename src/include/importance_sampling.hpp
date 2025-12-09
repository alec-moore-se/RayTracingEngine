#pragma once
#include "color.hpp"
#include "commons.hpp"
#include "hittable.hpp"
#include "vec3.hpp"

namespace importance_sampling {

// the pdf to the light from p
// unused
inline double pdf_of_light(const point3 &p, const vec3 &p_norm, const point3 &q,
                           double light_area) {
  auto to_light = normalize(q - p);
  auto cos_theta = dot_product(p_norm, to_light);
  return (std::pow(distance(p, q), 2)) / (std::cos(cos_theta) * light_area);
}

// with above
inline vec3 cosine_weighted_random_direction() {
  auto u1 = random_double();
  auto u2 = random_double();
  auto theta = 2 * PI * u1;
  return vec3(std::cos(theta) * std::sqrt(u2), std::sin(theta) * std::sqrt(u2),
              std::sqrt(1 - u2));
}

// unused -- from lecture notes
inline color lambertian_to_light(const color &c, double rays_ratio,
                                 double lambertian_pdf, double light_pdf) {
  auto beta = 1 - rays_ratio;
  return c / (rays_ratio * lambertian_pdf + beta * light_pdf);
}

// onb funcs
inline std::array<vec3, 3> orthonormal_basis(const vec3 &n) {
  auto w = unit_vector(n);
  vec3 a = (std::fabs(w.x()) > 0.9) ? vec3(0, 1, 0) : vec3(1, 0, 0);
  vec3 v = cross_product(w, a);
  auto u = cross_product(w, v);
  return std::array<vec3, 3>{u, v, w};
}

inline vec3 transform_onb(const vec3 &v, const std::array<vec3, 3> &basis) {
  return basis[0] * v.x() + basis[1] * v.y() + basis[2] * v.z();
}
}; // namespace importance_sampling

using namespace importance_sampling;
class PDF {
public:
  virtual ~PDF() = default;
  virtual double value(const vec3 &v) const = 0;
  virtual vec3 generate() const = 0;
};

class hittable_pdf : public PDF {
  const hittable &h;
  point3 o;

public:
  hittable_pdf(const hittable &h, const point3 &o) : h(h), o(o) {}
  double value(const vec3 &v) const override { return h.pdf_value(o, v); }
  vec3 generate() const override { return h.random(o); }
};

class Sphere_PDF : public PDF {
public:
  Sphere_PDF() {}
  double value(const vec3 &v) const override {
    (void)v;
    return 1 / (4 * PI);
  }
  vec3 generate() const override { return random_unit_vector(); }
};

class Cosine_PDF : public PDF {
  std::array<vec3, 3> onb;

public:
  Cosine_PDF(const vec3 &n) { onb = orthonormal_basis(n); }
  double value(const vec3 &v) const override {
    auto theta = dot_product(unit_vector(v), onb[2]);
    return std::fmax(0, theta / PI);
  }
  vec3 generate() const override {
    return transform_onb(cosine_weighted_random_direction(), onb);
  }
};

class Mixture_PDF : public PDF {
  std::array<shared_ptr<PDF>, 2> p;

public:
  Mixture_PDF(shared_ptr<PDF> p1, shared_ptr<PDF> p2) {
    p[0] = p1;
    p[1] = p2;
  }
  double value(const vec3 &v) const override {
    return (0.5 * p[0]->value(v)) + (0.5 * p[1]->value(v));
  }
  vec3 generate() const override {
    if (random_double() < 0.5)
      return p[0]->generate();
    else
      return p[1]->generate();
  }
};
