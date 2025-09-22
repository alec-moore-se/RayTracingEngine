
#include "../include/interval.hpp"
#include "../include/commons.hpp"

const interval interval::empty = interval(+infinity, -infinity);
const interval interval::universe = interval(-infinity, +infinity);
