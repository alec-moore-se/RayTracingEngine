#pragma once

#define TINYOBJLOADER_IMPLEMENTATION
#include "external/tiny_obj_loader.h"

#include "commons.hpp"
#include "hittable.hpp"
#include "material.hpp"
#include "vec3.hpp"

class triangle : public hittable {
  AABB aabb;
  vec3 a, b, c;
  shared_ptr<material> mat;
  double area;

public:
  triangle(vec3 a, vec3 b, vec3 c, shared_ptr<material> m)
      : a(a), b(b), c(c), mat(m) {
    triangle_min_max();
    auto disAB = distance(a, b);
    auto disBC = distance(b, c);
    auto disCA = distance(c, a);
    auto s = (disAB + disBC + disCA) / 2;
    area = sqrt(s * (s - disAB) * (s - disBC) * (s - disCA));
  }

  // testing constructor -- not used
  triangle(const point3 &p, double size, shared_ptr<material> m) {
    vec3 p1 = vec3(-size / 2.0, -sqrt(3) * size / 6.0, 0);
    vec3 p2 = vec3(size / 2.0, -sqrt(3) * size / 6.0, 0);
    vec3 p3 = vec3(0, sqrt(3) * size / 3.0, 0);
    a = p1 + p;
    b = p2 + p;
    c = p3 + p;
    mat = m;
    triangle_min_max();
  }

  double pdf_value(const point3 &o, const vec3 &v) const override {
    hit_rec rec;
    if (!this->hit(ray(o, v), interval(0.0001, infinity), rec))
      return 0;

    auto distance_squared = rec.t * rec.t * v.length_squared();
    auto cosine = std::fabs(dot_product(v, rec.norm) / v.length());
    return distance_squared / (cosine * area);
  }

  vec3 random(const point3 &o) const override {
    auto p = a + (random_double() * b) + (random_double() * c);
    return p - o;
  }

  bool hit(const ray &r, interval ray_t, hit_rec &rec) const override {
#ifdef BACKFACE_CULLING
    return intersect_with_backface(r, ray_t, rec);
#else
    return intersect_without_backface(r, ray_t, rec);
#endif
  }

  const vec3 &geta() const { return a; }
  const vec3 &getb() const { return b; }
  const vec3 &getc() const { return c; }

  AABB bounding_box() const override { return aabb; }

  bool intersect_without_backface(const ray &r, interval ray_t,
                                  hit_rec &rec) const {
    (void)ray_t;
    vec3 E1, E2, t, p, q;
    double det, i_det;
    E1 = (b - a);
    E2 = (c - a);
    p = cross_product(r.direction(), E2);
    det = dot_product(E1, p);
    if (det < epslion_flo)
      return false;

    t = (r.origin() - a);

    rec.u = dot_product(t, p);
    if (rec.u < 0.0 || rec.u > det)
      return false;

    q = cross_product(t, E1);

    rec.v = dot_product(r.direction(), q);
    if (rec.v < 0.0 || rec.u + rec.v > det) {
      return false;
    }

    rec.t = dot_product(E2, q);
    i_det = 1.0 / det;
    rec.t *= i_det;
    rec.u *= i_det;
    rec.v *= i_det;
    rec.set_face_normal(r, unit_vector(cross_product(E1, E2)));
    rec.mat = mat;

    return true;
  }

  bool intersect_with_backface(const ray &r, interval ray_t,
                               hit_rec &rec) const {
    (void)ray_t;
    vec3 E1, E2, p, t, q;
    double det, i_det;
    E1 = b - a;
    E2 = c - a;
    p = cross_product(r.direction(), E2);
    det = dot_product(E1, p);

    if (det > -epslion_flo && det < epslion_flo) {
      return false;
    }
    i_det = 1.0 / det;

    t = r.origin() - a;
    rec.u = dot_product(t, p) * i_det;
    if (rec.u < 0.0 || rec.u > 1.0)
      return false;
    q = cross_product(t, E1);
    rec.v = dot_product(r.direction(), q) * i_det;
    if (rec.v < 0.0 || rec.u + rec.v > 1.0)
      return false;
    rec.t = dot_product(E2, q) * i_det;

    rec.set_face_normal(r, unit_vector(cross_product(E1, E2)));
    rec.mat = mat;

    return true;
  }

private:
  inline void triangle_min_max() {
    double minx = a.x(), miny = a.y(), minz = a.z();
    double maxx = a.x(), maxy = a.y(), maxz = a.z();

    minx = (minx < b.x() && minx < c.x()) ? minx
           : (b.x() < c.x())              ? b.x()
                                          : c.x();
    maxx = (maxx > b.x() && maxx > c.x()) ? maxx
           : (b.x() > c.x())              ? b.x()
                                          : c.x();
    miny = (miny < b.y() && miny < c.y()) ? miny
           : (b.y() < c.y())              ? b.y()
                                          : c.y();
    maxy = (maxy > b.y() && maxy > c.y()) ? maxy
           : (b.y() > c.y())              ? b.y()
                                          : c.y();
    minz = (minz < b.z() && minz < c.z()) ? minz
           : (b.z() < c.z())              ? b.z()
                                          : c.z();
    maxz = (maxz > b.z() && maxz > c.z()) ? maxz
           : (b.z() > c.z())              ? b.z()
                                          : c.z();

    aabb = AABB(vec3(minx, miny, minz), vec3(maxx, maxy, maxz));
  }
};

inline void save_mesh_as_obj(const std::vector<triangle> &tris,
                             const std::string &filename) {
  std::ofstream out(filename);
  if (!out) {
    throw std::runtime_error("Unable to open OBJ file for writing: " +
                             filename);
  }

  for (const auto &t : tris) {
    out << "v " << t.geta().x() << " " << t.geta().y() << " " << t.geta().z()
        << "\n";
    out << "v " << t.getb().x() << " " << t.getb().y() << " " << t.getb().z()
        << "\n";
    out << "v " << t.getc().x() << " " << t.getc().y() << " " << t.getc().z()
        << "\n";
  }

  int tri_count = tris.size();
  for (int i = 0; i < tri_count; i++) {
    int i1 = i * 3 + 1;
    int i2 = i * 3 + 2;
    int i3 = i * 3 + 3;
    out << "f " << i1 << " " << i2 << " " << i3 << "\n";
  }

  out.close();
}

inline std::vector<triangle> load_mesh(const std::string &filename,
                                       shared_ptr<material> default_material) {
  std::vector<triangle> tris;

  tinyobj::ObjReaderConfig reader_config;
  tinyobj::ObjReader reader;

  if (!reader.ParseFromFile(filename, reader_config)) {
    throw std::runtime_error("Failed to load OBJ: " + filename);
  }

  const auto &attrib = reader.GetAttrib();
  const auto &shapes = reader.GetShapes();

  for (const auto &shape : shapes) {
    size_t index_offset = 0;

    for (size_t f = 0; f < shape.mesh.num_face_vertices.size(); f++) {
      int fv = shape.mesh.num_face_vertices[f];

      if (fv != 3)
        continue;

      vec3 v[3];

      for (int i = 0; i < 3; i++) {
        tinyobj::index_t idx = shape.mesh.indices[index_offset + i];

        float vx = attrib.vertices[3 * idx.vertex_index + 0];
        float vy = attrib.vertices[3 * idx.vertex_index + 1];
        float vz = attrib.vertices[3 * idx.vertex_index + 2];

        v[i] = vec3(vx, vz, -vy);
      }

      tris.emplace_back(v[0], v[1], v[2], default_material);

      index_offset += 3;
    }
  }

  return tris;
}
