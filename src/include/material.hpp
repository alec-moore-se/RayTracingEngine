#pragma once

#include "color.hpp"
#include "commons.hpp"
#include "hittable.hpp"
#include "vec3.hpp"

struct material {
  virtual ~material() = default;
  virtual bool scatter(const ray &r_in, const hit_rec &rec, color &attenuation,
                       ray &scattered) const = 0;
};

class lambertian : public material {
  color albedo;

public:
  lambertian(const color &albedo) : albedo(albedo) {}

  virtual bool scatter(const ray &r_in, const hit_rec &rec, color &attenuation,
                       ray &scattered) const override {
    auto scatter_direction = rec.norm + random_unit_vector();
    if (scatter_direction.near_zero())
      scatter_direction = rec.norm;

    scattered = ray(rec.p, scatter_direction, r_in.time());
    attenuation = albedo;
    return true;
  }
};

class metal : public material {
  color albedo;
  double fuzz;

public:
  metal(const color &albedo, double fuzz)
      : albedo(albedo), fuzz(fuzz < 1 ? fuzz : 1) {}

  virtual bool scatter(const ray &r_in, const hit_rec &rec, color &attenuation,
                       ray &scattered) const override {
    auto reflect_ = reflect(r_in.direction(), rec.norm);
    reflect_ = unit_vector(reflect_) + (fuzz * random_unit_vector());
    scattered = ray(rec.p, reflect_, r_in.time());
    attenuation = albedo;
    return (dot_product(scattered.direction(), rec.norm) > 0);
  }
};

class dielectric : public material {
  double refraction_i;

  static double reflectance(double cos, double refraction_i) {
    auto r = (1 - refraction_i) / (1 + refraction_i);
    r = r * r;
    return r + (1 - r) * std::pow((1 - cos), 5);
  }

public:
  dielectric(double refrac) : refraction_i(refrac) {}
  virtual bool scatter(const ray &r_in, const hit_rec &rec, color &attenuation,
                       ray &scattered) const override {
    attenuation = color(1.0, 1.0, 1.0);
    double ri = rec.front_face ? (1.0 / refraction_i) : refraction_i;
    vec3 unit_direction = unit_vector(r_in.direction());
    double cos_theta = std::fmin(dot_product(-unit_direction, rec.norm), 1.0);
    double sin_theta = std::sqrt(1.0 - cos_theta * cos_theta);

    bool cannot_refract = ri * sin_theta > 1.0;
    vec3 direction;

    if (cannot_refract || reflectance(cos_theta, ri) > random_double())
      direction = reflect(unit_direction, rec.norm);
    else
      direction = refract(unit_direction, rec.norm, ri);
    scattered = ray(rec.p, direction, r_in.time());
    return true;
  }
};
