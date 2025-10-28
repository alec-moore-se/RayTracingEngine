#include "commons.hpp"
#include "vec3.hpp"

class perlin {
  static const int p_count = 256;
  vec3 randVec[p_count];
  int perm_x[p_count];
  int perm_y[p_count];
  int perm_z[p_count];

private:
  static void perlin_generate_perm(int *p) {
    int i;
    for (i = 0; i < p_count; i++) {
      p[i] = i;
    }
    permute(p, p_count);
  }
  static void permute(int *p, int n = p_count) {
    int i;
    for (i = n - 1; i > 0; i--) {
      int target = random_int(0, i);
      int tmp = p[i];
      p[i] = p[target];
      p[target] = tmp;
    }
  }

  static double trilinear_interpolate(double d[2][2][2], double u, double v,
                                      double w) {
    u = u * u * (3 - 2 * u);
    v = v * v * (3 - 2 * v);
    w = w * w * (3 - 2 * w);
    double accumulater = 0.0;
    for (int i = 0; i < 2; i++)
      for (int j = 0; j < 2; j++)
        for (int k = 0; k < 2; k++)
          accumulater += (i * u + (1 - i) * (1 - u)) *
                         (j * v + (1 - j) * (1 - v)) *
                         (k * w + (1 - k) * (1 - w)) * d[i][j][k];
    return accumulater;
  }
  static double vec3_interpolate(vec3 d[2][2][2], double u, double v,
                                 double w) {
    double a = u * u * (3 - 2 * u);
    double b = v * v * (3 - 2 * v);
    double c = w * w * (3 - 2 * w);
    double accumulater = 0.0;
    for (int i = 0; i < 2; i++)
      for (int j = 0; j < 2; j++)
        for (int k = 0; k < 2; k++) {

          vec3 weight(u - i, v - j, w - k);
          accumulater +=
              (i * a + (1 - i) * (1 - a)) * (j * b + (1 - j) * (1 - b)) *
              (k * c + (1 - k) * (1 - c)) * dot_product(d[i][j][k], weight);
        }
    return accumulater;
  }

public:
  perlin() {
    for (int i = 0; i < p_count; i++) {
      randVec[i] = unit_vector(vec3::random(-1, 1));
    }
    perlin_generate_perm(perm_x);
    perlin_generate_perm(perm_y);
    perlin_generate_perm(perm_z);
  }

  double noise(const point3 &p) const {
    int a, b, c, i, j, k;
    double u = p.x() - std::floor(p.x());
    double v = p.y() - std::floor(p.y());
    double w = p.z() - std::floor(p.z());

    i = int(std::floor(p.x()));
    j = int(std::floor(p.y()));
    k = int(std::floor(p.z()));

    vec3 d[2][2][2];
    for (a = 0; a < 2; a++)
      for (b = 0; b < 2; b++)
        for (c = 0; c < 2; c++)
          d[a][b][c] = randVec[perm_x[(i + a) & 255] ^ perm_y[(j + b) & 255] ^
                               perm_z[(k + c) & 255]];

    return vec3_interpolate(d, u, v, w);
  }

  double turbulence(point3 p, int depth = 7, double added_weight = 0.5) const {
    int i;
    double accumulater = 0.0;
    double weight = 1.0;

    for (i = 0; i < depth; i++) {
      accumulater += weight * noise(p);
      weight *= added_weight;
      p *= 2;
    }
    return std::fabs(accumulater);
  }
};
