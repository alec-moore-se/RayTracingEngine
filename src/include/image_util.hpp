#pragma once

#define STB_IMAGE_IMPLEMENTATION
#define STB_FAILURE_USRMSG
#include "external/stb_image.h"

#include "commons.hpp"
#include <cstdlib>

class image_util {
  const int bytes_per_pixel = 3;
  float *fdata = nullptr;
  unsigned char *bdata = nullptr;
  int width;
  int height;
  int bytes_per_scanline;

public:
  image_util() {}
  image_util(const char *filename) {
    std::string filename_ = std::string(filename);
    auto image_dir = getenv("../images");

    if (image_dir && load(std::string(image_dir) + "/" + filename))
      return;
    if (load(filename_))
      return;
    if (load("images/" + filename_))
      return;
    if (load("../images/" + filename_))
      return;
    if (load("../../images/" + filename_))
      return;
  }
  ~image_util() {
    delete[] bdata;
    STBI_FREE(fdata);
  }

  bool load(const std::string &filename) {
    auto n = bytes_per_pixel;
    fdata = stbi_loadf(filename.c_str(), &width, &height, &n, bytes_per_pixel);
    if (fdata == nullptr) {
      return false;
    }

    bytes_per_scanline = width * bytes_per_pixel;
    convert_bytes();
    return true;
  }

  int get_width() const { return (fdata == nullptr) ? 0 : width; }
  int get_height() const { return (fdata == nullptr) ? 0 : height; }

  const unsigned char *pixel_data(int x, int y) const {
    static unsigned char magenta[] = {255, 0, 255};
    if (bdata == nullptr)
      return magenta;
    x = clamp(x, 0, width);
    y = clamp(y, 0, height);

    return bdata + y * bytes_per_scanline + x * bytes_per_pixel;
  }

private:
  static unsigned char float_byte(float v) {
    return (v <= 0.0)   ? 0
           : (v >= 1.0) ? 255
                        : static_cast<unsigned char>(v * 256.0);
  }

  void convert_bytes() {
    // floating point --> color_bytes
    int total = width * height * bytes_per_pixel;
    bdata = new unsigned char[total];

    // convert [0.0, 1.0] --> [0, 255]
    auto *bptr = bdata;
    auto *fptr = fdata;
    for (auto i = 0; i < total; i++, fptr++, bptr++) {
      *bptr = float_byte(*fptr);
    }
  }
};
