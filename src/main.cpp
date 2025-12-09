#include "include/bounding_volume_hierarchies.hpp"
#include "include/camera.hpp"
#include "include/commons.hpp"
#include "include/hittable_list.hpp"
#include "include/material.hpp"
#include "include/quadrilaterals.hpp"
#include "include/quaternian.hpp"
#include "include/sphere.hpp"
#include "include/texture.hpp"
#include "include/translate.hpp"
#include "include/triangle.hpp"
#include "include/volume.hpp"
#include <gtest/gtest.h>

auto empty_light = shared_ptr<material>();
auto none_light = quadrilateral(point3(0, 0, 0), vec3(), vec3(), empty_light);

void bouncing_spheres() {

  hittable_list w;
  auto checker =
      make_shared<Checker_Texture>(0.32, color(.2, .3, .1), color(.9, .9, .9));
  w.add(make_shared<sphere>(point3(0, -1000, 0), 1000,
                            make_shared<lambertian>(checker)));
  for (int a = -11; a < 11; a++) {
    for (int b = -11; b < 11; b++) {
      auto choose_mat = random_double();
      point3 center(a + 0.9 * random_double(), 0.2, b + 0.9 * random_double());

      if ((center - point3(4, 0.2, 0)).length() > 0.9) {
        shared_ptr<material> sphere_material;
        shared_ptr<material> triangle_material;

        if (choose_mat < 0.8) {
          // diffuse
          auto albedo = color::random() * color::random();
          sphere_material = make_shared<lambertian>(albedo);
          auto center2 = center + vec3(0, random_double(0, .5), 0);
          w.add(make_shared<sphere>(center, center2, 0.2, sphere_material));
        } else if (choose_mat < 0.95) {
          // metal
          auto albedo = color::random(0.5, 1);
          auto fuzz = random_double(0, 0.5);
          auto sphere_material = make_shared<metal>(albedo, fuzz);
          w.add(make_shared<sphere>(center, 0.2, sphere_material));

        } else {
          // glass
          sphere_material = make_shared<dielectric>(1.5);
          w.add(make_shared<sphere>(center, 0.2, sphere_material));
        }
      }
    }
  }

  auto material1 = make_shared<dielectric>(1.5);
  w.add(make_shared<sphere>(point3(0, 1, 0), 1.0, material1));

  auto material2 = make_shared<lambertian>(color(0.4, 0.2, 0.1));
  w.add(make_shared<sphere>(point3(-4, 1, 0), 1.0, material2));

  auto material3 = make_shared<metal>(color(0.7, 0.6, 0.5), 0.0);
  w.add(make_shared<sphere>(point3(4, 1, 0), 1.0, material3));

  w = hittable_list(make_shared<BVH>(w));

  camera cam;

  cam.aspect_ratio = 16.0 / 9.0;
  cam.image_width = 600;
  cam.samples_per_pixel = 20;
  cam.max_depth = 15;

  cam.vfov = 50;
  cam.lookfrom = point3(8, 1, 3);
  cam.lookat = point3(0, 0, 0);
  cam.vup = vec3(0, 1, 0);

  cam.defocus_angle = 0.6;
  cam.focus_dist = 10.0;
  cam.background_color = color(0.7, .8, 1);

  cam.threaded_render(w, none_light);
}

void testingScene() {
  hittable_list world;

  auto mat = make_shared<lambertian>(color(0.6, 0.7, 0.9));
  auto tall = box(point3(-0.5, 0, -0.5), point3(0.5, 4, 0.5), mat);

  auto r1 = make_shared<rotate>(tall, rotZ, 45);

  auto r2 = make_shared<rotate>(r1, rotX, 45);

  auto r3 = make_shared<rotate>(r2, rotY, 45);

  world.add(r3);

  camera cam;
  cam.aspect_ratio = 16.0 / 9.0;
  cam.image_width = 400;
  cam.samples_per_pixel = 100;
  cam.max_depth = 20;

  cam.vfov = 40;
  cam.lookfrom = point3(5, 4, 6);
  cam.lookat = point3(0, 1.5, 0);
  cam.vup = vec3(0, 1, 0);

  cam.background_color = color(0.6, 0.7, 1.0);

  cam.threaded_render(world, none_light);
}

void triangles() {
  hittable_list w;

  auto checker =
      make_shared<Checker_Texture>(0.32, color(.2, .3, .1), color(.9, .9, .9));
  w.add(make_shared<sphere>(point3(0, -1000, 0), 1000,
                            make_shared<lambertian>(checker)));

  auto material1 = make_shared<lambertian>(color(1.5, 254, 254));
  w.add(make_shared<triangle>(point3(0, 1, 0), 2.0, material1));

  auto material2 = make_shared<lambertian>(color(0.4, 0.2, 0.1));
  w.add(make_shared<triangle>(point3(-4, 1, 0), 2.0, material2));

  auto material3 = make_shared<lambertian>(color(0.7, 0.6, 0.5));
  w.add(make_shared<triangle>(point3(4, 1, 0), 2.0, material3));

  w = hittable_list(make_shared<BVH>(w));

  camera cam;

  cam.aspect_ratio = 16.0 / 9.0;
  cam.image_width = 600;
  cam.samples_per_pixel = 20;
  cam.max_depth = 15;

  cam.vfov = 20;
  cam.lookfrom = point3(5, 1, 3);
  cam.lookat = point3(0, 1, 0);
  cam.vup = vec3(0, 1, 0);

  cam.defocus_angle = 0.6;
  cam.focus_dist = 10.0;
  cam.background_color = color(0.7, .8, 1);

  cam.threaded_render(w, none_light);
}

void earth() {
  auto earth_texture = make_shared<Image_Texture>("earthmap.jpg");
  auto earth_surface = make_shared<lambertian>(earth_texture);
  auto globe = make_shared<sphere>(point3(0, 0, 0), 2, earth_surface);

  camera cam;

  cam.aspect_ratio = 16.0 / 9.0;
  cam.image_width = 400;
  cam.samples_per_pixel = 100;
  cam.max_depth = 50;

  cam.vfov = 20;
  cam.lookfrom = point3(0, 0, 12);
  cam.lookat = point3(0, 0, 0);
  cam.vup = vec3(0, 1, 0);

  cam.defocus_angle = 0;
  cam.background_color = color(0.7, .8, 1);

  cam.threaded_render(hittable_list(globe), none_light);
}

void perlin_spheres() {
  hittable_list world;
  Noise_Texture perlin_sphere(4, Noise_Texture::TURBULENT);
  auto pertext = make_shared<Noise_Texture>(perlin_sphere);
  world.add(make_shared<sphere>(point3(0, -1000, 0), 1000,
                                make_shared<lambertian>(pertext)));
  world.add(make_shared<sphere>(point3(0, 2, 0), 2,
                                make_shared<lambertian>(pertext)));

  camera cam;

  cam.aspect_ratio = 16.0 / 9.0;
  cam.image_width = 400;
  cam.samples_per_pixel = 10;
  cam.max_depth = 50;

  cam.vfov = 20;
  cam.lookfrom = point3(13, 2, 3);
  cam.lookat = point3(0, 0, 0);
  cam.vup = vec3(0, 1, 0);

  cam.defocus_angle = 0;
  cam.background_color = color(0.7, .8, 1);

  cam.threaded_render(world, none_light);
}
void quadrilaterals() {
  hittable_list world;

  shared_ptr<material> left_red = make_shared<lambertian>(color(1.0, 0.2, 0.2));
  shared_ptr<material> back_green =
      make_shared<lambertian>(color(0.2, 1.0, 0.2));
  shared_ptr<material> right_blue =
      make_shared<lambertian>(color(0.2, 0.2, 1.0));
  shared_ptr<material> upper_orange =
      make_shared<lambertian>(color(1.0, 0.5, 0.0));
  shared_ptr<material> lower_teal =
      make_shared<lambertian>(color(0.2, 0.8, 0.8));

  world.add(make_shared<quadrilateral>(point3(-3, -2, 5), vec3(0, 0, -4),
                                       vec3(0, 4, 0), left_red));
  world.add(make_shared<quadrilateral>(point3(-2, -2, 0), vec3(4, 0, 0),
                                       vec3(0, 4, 0), back_green));
  world.add(make_shared<quadrilateral>(point3(3, -2, 1), vec3(0, 0, 4),
                                       vec3(0, 4, 0), right_blue));
  world.add(make_shared<quadrilateral>(point3(-2, 3, 1), vec3(4, 0, 0),
                                       vec3(0, 0, 4), upper_orange));
  world.add(make_shared<quadrilateral>(point3(-2, -3, 5), vec3(4, 0, 0),
                                       vec3(0, 0, -4), lower_teal));

  camera cam;

  cam.aspect_ratio = 1.0;
  cam.image_width = 400;
  cam.samples_per_pixel = 100;
  cam.max_depth = 50;

  cam.vfov = 80;
  cam.lookfrom = point3(0, 0, 9);
  cam.lookat = point3(0, 0, 0);
  cam.vup = vec3(0, 1, 0);

  cam.defocus_angle = 0;
  cam.background_color = color(0.7, .8, 1);

  cam.threaded_render(world, none_light);
}

void simple_light() {
  hittable_list world;

  auto pertext = make_shared<Noise_Texture>(4, Noise_Texture::MARBLE);
  world.add(make_shared<sphere>(point3(0, -1000, 0), 1000,
                                make_shared<lambertian>(pertext)));
  world.add(make_shared<sphere>(point3(0, 2, 0), 2,
                                make_shared<lambertian>(pertext)));

  auto difflight = make_shared<Diffuse>(color(4, 4, 4));
  auto light = make_shared<quadrilateral>(point3(3, 1, -2), vec3(2, 0, 0),
                                          vec3(0, 2, 0), difflight);
  world.add(light);

  camera cam;

  cam.aspect_ratio = 16.0 / 9.0;
  cam.image_width = 400;
  cam.samples_per_pixel = 100;
  cam.max_depth = 50;
  cam.background_color = color(0, 0, 0);

  cam.vfov = 20;
  cam.lookfrom = point3(26, 3, 6);
  cam.lookat = point3(0, 2, 0);
  cam.vup = vec3(0, 1, 0);

  cam.defocus_angle = 0;

  cam.threaded_render(world, *light);
}

void cornell_box() {
  hittable_list world;
  hittable_list lights;

  auto red = make_shared<lambertian>(color(.65, .05, .05));
  auto white = make_shared<lambertian>(color(.73, .73, .73));
  auto green = make_shared<lambertian>(color(.12, .45, .15));
  auto light = make_shared<Diffuse>(color(15, 15, 15));

  world.add(make_shared<quadrilateral>(point3(555, 0, 0), vec3(0, 555, 0),
                                       vec3(0, 0, 555), green));
  world.add(make_shared<quadrilateral>(point3(0, 0, 0), vec3(0, 555, 0),
                                       vec3(0, 0, 555), red));
  auto quad_light = make_shared<quadrilateral>(
      point3(343, 554, 332), vec3(-130, 0, 0), vec3(0, 0, -105), light);
  world.add(quad_light);
  lights.add(quad_light);
  world.add(make_shared<quadrilateral>(point3(0, 0, 0), vec3(555, 0, 0),
                                       vec3(0, 0, 555), white));
  world.add(make_shared<quadrilateral>(point3(555, 555, 555), vec3(-555, 0, 0),
                                       vec3(0, 0, -555), white));
  world.add(make_shared<quadrilateral>(point3(0, 0, 555), vec3(555, 0, 0),
                                       vec3(0, 555, 0), white));

  world.add(box(point3(130, 0, 65), point3(295, 165, 230), white));
  world.add(box(point3(265, 0, 295), point3(430, 330, 460), white));
  shared_ptr<material> aluminum =
      make_shared<metal>(color(0.8, 0.85, 0.88), 0.0);
  shared_ptr<hittable> box1 =
      box(point3(0, 0, 0), point3(165, 330, 165), aluminum);
  box1 = make_shared<rotate>(box1, rotY, 15);
  box1 = make_shared<translate>(box1, vec3(265, 0, 295));
  world.add(box1);

  shared_ptr<hittable> box2 =
      box(point3(0, 0, 0), point3(165, 165, 165), white);
  box2 = make_shared<rotate>(box2, rotY, -18);
  box2 = make_shared<translate>(box2, vec3(130, 0, 65));
  world.add(box2);

  auto glass = make_shared<dielectric>(1.5);
  world.add(make_shared<sphere>(point3(460, 90, 50), 90, glass));
  // lights.add(make_shared<sphere>(point3(90, 90, 190), 90, glass));

  camera cam;

  cam.aspect_ratio = 1.0;
  cam.image_width = 200;
  cam.samples_per_pixel = 1000;
  cam.max_depth = 150;
  cam.background_color = color(0, 0, 0);

  cam.vfov = 40;
  cam.lookfrom = point3(278, 278, -800);
  cam.lookat = point3(278, 278, 0);
  cam.vup = vec3(0, 1, 0);

  cam.defocus_angle = 0;

  cam.threaded_render(world, lights);
}

void final_scene(int image_width, int samples_per_pixel, int max_depth) {
  hittable_list boxes1 = hittable_list();
  hittable_list world;
  hittable_list lights;

  auto ground = make_shared<lambertian>(color(0.48, 0.83, 0.53));

  int boxes_per_side = 20;
  for (int i = 0; i < boxes_per_side; i++) {
    for (int j = 0; j < boxes_per_side; j++) {
      auto w = 100.0;
      auto x0 = -1000.0 + i * w;
      auto z0 = -1000.0 + j * w;
      auto y0 = 0.0;
      auto x1 = x0 + w;
      auto y1 = random_double(1, 101);
      auto z1 = z0 + w;

      boxes1.add(box(point3(x0, y0, z0), point3(x1, y1, z1), ground));
    }
  }

  // makes an exploding box
  // looks coolish, just not "realistic"
  auto pure_white = make_shared<lambertian>(color(1, 1, 1));
  shared_ptr<hittable> box_smoke =
      box(point3(340, 170, 145), point3(465, 330, 165), pure_white);
  //  auto box_smoke =
  //      box(point3(-10000000, -10000000, -10000000),
  //          point3(10000000, 10000000, 10000000), pure_white);
  world.add(make_shared<absorption>(box_smoke, 0.000001, color(244, 30, 0)));

  world.add(make_shared<BVH>(boxes1));

  auto light = make_shared<Diffuse>(color(7, 7, 7));
  // different lighting
  auto tri_light = make_shared<triangle>(
      point3(0, 500, 200), vec3(500, 400, 700), vec3(800, 600, 100), light);
  auto empty_tri_light =
      make_shared<triangle>(point3(0, 500, 200), vec3(500, 400, 700),
                            vec3(800, 600, 100), empty_light);
  world.add(tri_light);
  lights.add(empty_tri_light);

  // motion blur on sphere
  auto center1 = point3(400, 400, 200);
  auto center2 = center1 + vec3(30, 0, 0);
  auto sphere_material = make_shared<lambertian>(color(0.7, 0.3, 0.1));
  world.add(make_shared<sphere>(center1, center2, 50, sphere_material));

  world.add(make_shared<sphere>(point3(260, 150, 45), 50,
                                make_shared<dielectric>(1.5)));
  // having bad interactions with glass in the lights list -
  // lights.add(make_shared<sphere>(point3(260, 150, 45), 50,
  // make_shared<dielectric>(1.5)));
  world.add(make_shared<sphere>(point3(0, 150, 145), 50,
                                make_shared<metal>(color(0.8, 0.8, 0.9), 1.0)));

  auto boundary = make_shared<sphere>(point3(360, 150, 145), 70,
                                      make_shared<dielectric>(1.5));
  world.add(boundary);
  world.add(make_shared<absorption>(boundary, 0.2, color(0.2, 0.4, 0.9)));
  boundary =
      make_shared<sphere>(point3(0, 0, 0), 5000, make_shared<dielectric>(1.5));
  world.add(make_shared<absorption>(boundary, .0001, color(1, 1, 1)));

  auto emat =
      make_shared<lambertian>(make_shared<Image_Texture>("earthmap.jpg"));
  auto emat2 =
      make_shared<Diffuse>(make_shared<Image_Texture>("kagurabachi.png"));
  world.add(make_shared<sphere>(point3(400, 200, 300), 2, emat));
  // fun + image on a quad
  auto kag = make_shared<quadrilateral>(
      point3(-223, 154, 9547), vec3(600, 0, 0), vec3(0, 665, 0), emat2);
  world.add(kag);
  // technically a light source, but do not want
  // rays to go towards it lights.add(kag);

  auto pertext = make_shared<Noise_Texture>(4, Noise_Texture::MARBLE);
  world.add(make_shared<sphere>(point3(220, 280, 300), 80,
                                make_shared<lambertian>(pertext)));

  hittable_list boxes2;
  auto white = make_shared<lambertian>(color(.73, .73, .73));
  int ns = 1000;
  for (int j = 0; j < ns; j++) {
    boxes2.add(make_shared<sphere>(point3::random(0, 165), 10, white));
  }

  // translate and rotate with quaternions
  world.add(make_shared<translate>(
      make_shared<rotate>(make_shared<BVH>(boxes2), rotY, 15),
      vec3(-100, 270, 395)));

  camera cam;

  // camera aspects
  cam.aspect_ratio = 1;
  cam.image_width = image_width;
  cam.samples_per_pixel = samples_per_pixel;
  cam.max_depth = max_depth;
  cam.background_color = color(0, 0, 0);

  cam.vfov = 60;
  cam.lookfrom = point3(478, 278, -600);
  cam.lookat = point3(278, 278, 0);
  cam.vup = vec3(0, 1, 0);
  cam.defocus_angle = 0;

  cam.threaded_render(world, lights);
}

void cornell_smoke() {
  hittable_list world;

  auto red = make_shared<lambertian>(color(.65, .05, .05));
  auto white = make_shared<lambertian>(color(.73, .73, .73));
  auto green = make_shared<lambertian>(color(.12, .45, .15));
  auto light = make_shared<Diffuse>(color(7, 7, 7));

  world.add(make_shared<quadrilateral>(point3(555, 0, 0), vec3(0, 555, 0),
                                       vec3(0, 0, 555), green));
  world.add(make_shared<quadrilateral>(point3(0, 0, 0), vec3(0, 555, 0),
                                       vec3(0, 0, 555), red));
  auto quad_light = make_shared<quadrilateral>(
      point3(113, 554, 127), vec3(330, 0, 0), vec3(0, 0, 305), light);
  world.add(make_shared<quadrilateral>(point3(0, 555, 0), vec3(555, 0, 0),
                                       vec3(0, 0, 555), white));
  world.add(make_shared<quadrilateral>(point3(0, 0, 0), vec3(555, 0, 0),
                                       vec3(0, 0, 555), white));
  world.add(make_shared<quadrilateral>(point3(0, 0, 555), vec3(555, 0, 0),
                                       vec3(0, 555, 0), white));

  shared_ptr<hittable> box1 =
      box(point3(0, 0, 0), point3(165, 330, 165), white);

  shared_ptr<hittable> box2 =
      box(point3(0, 0, 0), point3(165, 165, 165), white);

  world.add(make_shared<absorption>(box1, 0.01, color(0, 0, 0)));
  world.add(make_shared<absorption>(box2, 0.01, color(1, 1, 1)));

  camera cam;

  cam.aspect_ratio = 1.0;
  cam.image_width = 600;
  cam.samples_per_pixel = 200;
  cam.max_depth = 50;
  cam.background_color = color(0, 0, 0);

  cam.vfov = 40;
  cam.lookfrom = point3(278, 278, -800);
  cam.lookat = point3(278, 278, 0);
  cam.vup = vec3(0, 1, 0);

  cam.defocus_angle = 0;

  cam.threaded_render(world, *quad_light);
}

void mesh_test_scene() {

  hittable_list world;

  auto ground_mat = make_shared<lambertian>(color(0.5, 0.5, 0.5));
  world.add(make_shared<sphere>(point3(0, -1000, 0), 1000, ground_mat));

  auto mesh_mat = make_shared<lambertian>(color(0.4, 0.7, 0.9));
  auto flat_mat = make_shared<lambertian>(color(0, 0, 0));
  world.add(make_shared<quadrilateral>(
      point3(-1000, -1000, -30), vec3(2000, 0, 0), vec3(0, 2000, 0), flat_mat));
  auto light = make_shared<Diffuse>(color(7, 7, 7));
  world.add(make_shared<quadrilateral>(point3(123, 554, 147), vec3(300, 0, 0),
                                       vec3(0, 0, 265), light));

  std::string path = "images/bunny.obj";

  std::vector<triangle> tris = load_mesh(path, mesh_mat);

  for (auto &t : tris)
    world.add(make_shared<triangle>(t));

  world = hittable_list(make_shared<BVH>(world));

  camera cam;

  cam.aspect_ratio = 16.0 / 9.0;
  cam.image_width = 400;
  cam.samples_per_pixel = 30;
  cam.max_depth = 10;

  cam.vfov = 40;
  cam.lookfrom = point3(2, 5, 11);
  cam.lookat = point3(0, 5, 0);
  cam.vup = vec3(0, 1, 0);

  cam.defocus_angle = 0.0;
  cam.focus_dist = 10.0;
  cam.background_color = color(0.7, 0.8, 1.0);

  cam.threaded_render(world, none_light);
}

int main(int argc, char *argv[]) {
  int picture_to_load = 8;
  if (argc > 1)
    picture_to_load = std::atoi(argv[1]);

  switch (picture_to_load) {
  case 0:
    bouncing_spheres();
    break;
  case 1:
    triangles();
    break;
  case 2:
    earth();
    break;
  case 3:
    perlin_spheres();
    break;
  case 4:
    quadrilaterals();
    break;
  case 5:
    simple_light();
    break;
  case 6:
    cornell_box();
    break;
  case 7:
    cornell_smoke();
    break;
  case 8:
    final_scene(400, 100, 200);
    break;
  case 9:
    mesh_test_scene();
    break;
  case 90:
    testingScene();
    break;
  case 99:
    testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
  }
}
