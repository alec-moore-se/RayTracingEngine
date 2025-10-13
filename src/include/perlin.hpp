#include "commons.hpp"
#include "vec3.hpp"

class perlin {
  static const int p_count = 256;
  double randdouble[p_count];
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
  static void permute(int *p, int n) {
    int i;
    for (i = n - 1; i > 0; i--) {
      int target = random_int(0, i);
      int tmp = p[i];
      p[i] = p[target];
      p[target] = tmp;
    }
  }

public:
  perlin() {
    for (int i = 0; i < p_count; i++) {
      randdouble[i] = random_double();
    }
    perlin_generate_perm(perm_x);
    perlin_generate_perm(perm_y);
    perlin_generate_perm(perm_z);
  }

  double noise(const point3 &p) const {
    int i = int(4 * p.x()) & 255;
    int j = int(4 * p.x()) & 255;
    int k = int(4 * p.x()) & 255;

    return randdouble[perm_x[i] ^ perm_y[j] ^ perm_z[k]];
  }
};
