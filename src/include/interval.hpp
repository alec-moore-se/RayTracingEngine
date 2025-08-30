#pragma once
#include "commons.hpp"
struct interval {
  double min, max;

  interval() : min(+infinity), max(-infinity) {}
  interval(double min, double max) : min(min), max(max) {}

  double size() const { return max - min; }

  bool contains(double x) const { return min <= x && max >= x; }
  bool surrounds(double x) const { return min < x && max > x; }

  static const interval empty, universe;
};
