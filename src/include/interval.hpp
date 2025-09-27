#pragma once
#include <algorithm>
struct interval {
  double min, max;

  interval() : interval(empty) {}
  interval(const interval &i) : interval(i.min, i.max) {}
  interval(double min, double max) : min(min), max(max) {}
  interval(const interval &a, const interval &b) {
    min = (a.min <= b.min) ? a.min : b.min;
    max = (a.max >= b.max) ? a.max : b.max;
  }

  double size() const { return max - min; }
  interval &operator=(const interval &v) = default;

  bool contains(double x) const { return min <= x && max >= x; }
  bool surrounds(double x) const { return min < x && max > x; }
  double clamp(double x) const {
    if (x < min)
      return min;
    if (x > max)
      return max;
    return x;
  }

  bool overlaps(interval t_interval1, interval t_interval2) {
    auto t_min = std::max(t_interval1.min, t_interval2.min);
    auto t_max = std::min(t_interval1.max, t_interval2.max);
    return t_min < t_max;
  }

  interval expand(double d) const {
    auto padding = d / 2;
    return interval(min - padding, max + padding);
  }

  static const interval empty, universe;
};
