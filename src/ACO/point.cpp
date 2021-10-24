#include <math.h>

#include "ACO/point.h"

namespace ACO {
  double get_dist(const Point &a, const Point &b) {
    return sqrt((a.x - b.x) * (a.x - b.x) + (a.y - b.y) * (a.y - b.y));
  }
}
