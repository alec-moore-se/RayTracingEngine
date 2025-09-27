#include "../include/aabb.hpp"

const AABB AABB::empty =
    AABB(interval::empty, interval::empty, interval::empty);
const AABB AABB::universe =
    AABB(interval::universe, interval::universe, interval::universe);
