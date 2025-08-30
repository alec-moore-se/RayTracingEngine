#include "include/camera.hpp"
#include "include/commons.hpp"
#include "include/hittable_list.hpp"
#include "include/sphere.hpp"

int main() {
  hittable_list world;
  camera cam;
  cam.aspect_ratio = 16.0 / 9.0;
  cam.image_width = 400;
  world.add(make_shared<sphere>(point3(0, 0, -1), 0.5));
  world.add(make_shared<sphere>(point3(0, -100.5, -1), 10));

  cam.render(world);
}
