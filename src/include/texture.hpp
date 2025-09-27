#include "color.hpp"

class Texture {
public:
  virtual ~Texture() = default;
  virtual color value(double u, double v, const point3 &p) const = 0;
};

class Solid_Color : public Texture {
  color albedo;

public:
  Solid_Color(const color &c) : albedo(c) {}
  Solid_Color(double r, double g, double b) : Solid_Color(color(r, g, b)) {}

  color value(double u, double v, const point3 &p) const override {
    (void)u;
    (void)v;
    (void)p;
    return albedo;
  }
};

class Checker_Texture : public Texture {
  double inv_scaler;
  shared_ptr<Texture> even;
  shared_ptr<Texture> odd;

public:
  Checker_Texture(double scaler, shared_ptr<Texture> even,
                  shared_ptr<Texture> odd)
      : inv_scaler(1 / scaler), even(even), odd(odd) {}
  Checker_Texture(double scaler, const color &c1, const color &c2)
      : inv_scaler(1 / scaler), even(make_shared<Solid_Color>(c1)),
        odd(make_shared<Solid_Color>(c2)) {}

  color value(double u, double v, const point3 &p) const override {

    int x = int(std::floor(inv_scaler * p.x()));
    int y = int(std::floor(inv_scaler * p.y()));
    int z = int(std::floor(inv_scaler * p.z()));
    bool even_ = (x + y + z) & 1;
    return even_ ? even->value(u, v, p) : odd->value(u, v, p);
  }
};
