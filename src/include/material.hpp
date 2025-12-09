#pragma once

#include "color.hpp"
#include "commons.hpp"
#include "hittable.hpp"
#include "importance_sampling.hpp"
#include "texture.hpp"
#include "vec3.hpp"
#include <memory>

class scatter_record {
public:
  color attenuation;
  shared_ptr<PDF> pdf_ptr;
  bool skip_pdf;
  ray skip_pdf_ray;
};

struct material {
  virtual ~material() = default;
  virtual bool scatter(const ray &r_in, const hit_rec &rec,
                       scatter_record &srec) const {
    (void)r_in;
    (void)rec;
    (void)srec;
    return false;
  };
  virtual double scattering_pdf(const ray &r_in, const hit_rec &rec,
                                const ray &scattered) const {
    (void)r_in;
    (void)rec;
    (void)scattered;
    return 0;
  }
  virtual color emitted(double u, double v, const point3 &p) const {
    (void)u;
    (void)v;
    (void)p;
    return color(0, 0, 0);
  }
};

class lambertian : public material {
  shared_ptr<Texture> tex;

public:
  lambertian(const color &albedo) : tex(make_shared<Solid_Color>(albedo)) {}
  lambertian(shared_ptr<Texture> tex) : tex(tex) {}

  bool scatter(const ray &r_in, const hit_rec &rec,
               scatter_record &srec) const override {
    (void)r_in;
    srec.attenuation = tex->value(rec.u, rec.v, rec.p);
    srec.pdf_ptr = make_shared<Cosine_PDF>(rec.norm);
    srec.skip_pdf = false;
    return true;
  }
  double scattering_pdf(const ray &r_in, const hit_rec &rec,
                        const ray &scattered) const override {
    (void)scattered;
    (void)r_in;
    auto cos_theta = dot_product(rec.norm, unit_vector(scattered.direction()));
    return (cos_theta <= 0) ? 0 : (cos_theta / PI);
  }
};

class metal : public material {
  color albedo;
  double fuzz;

public:
  metal(const color &albedo, double fuzz)
      : albedo(albedo), fuzz(fuzz < 1 ? fuzz : 1) {}

  virtual bool scatter(const ray &r_in, const hit_rec &rec,
                       scatter_record &srec) const override {
    vec3 reflect_ = reflect(r_in.direction(), rec.norm);
    reflect_ = unit_vector(reflect_) + (fuzz * random_unit_vector());
    srec.attenuation = albedo;
    srec.pdf_ptr = nullptr;
    srec.skip_pdf = true;
    srec.skip_pdf_ray = ray(rec.p, reflect_, r_in.time());
    return true;
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
  virtual bool scatter(const ray &r_in, const hit_rec &rec,
                       scatter_record &srec) const override {
    srec.attenuation = color(1.0, 1.0, 1.0);
    srec.skip_pdf = true;
    srec.pdf_ptr = nullptr;
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
    srec.skip_pdf_ray = ray(rec.p, direction, r_in.time());
    return true;
  }
};

class Diffuse : public material {
  shared_ptr<Texture> tex;

public:
  Diffuse(shared_ptr<Texture> tex) : tex(tex) {}
  Diffuse(const color &emit) : tex(make_shared<Solid_Color>(emit)) {}

  color emitted(double u, double v, const point3 &p) const override {
    return tex->value(u, v, p);
  }
};

class isotropic : public material {
  shared_ptr<Texture> tex;

public:
  isotropic(const color &albedo) : tex(make_shared<Solid_Color>(albedo)) {}
  isotropic(shared_ptr<Texture> tex) : tex(tex) {}

  virtual bool scatter(const ray &r_in, const hit_rec &rec,
                       scatter_record &srec) const override {
    (void)r_in;
    srec.attenuation = tex->value(rec.u, rec.v, rec.p);
    srec.pdf_ptr = make_shared<Sphere_PDF>();
    srec.skip_pdf = false;
    return true;
  }

  virtual double scattering_pdf(const ray &r_in, const hit_rec &rec,
                                const ray &scattered) const override {
    (void)r_in;
    (void)rec;
    (void)scattered;
    return 1 / (4 * PI);
  }
};
