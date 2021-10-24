#include "util.h"

namespace ACO {
  double pow(double x, int y) {
    double res = 1;
    while (y) {
      if (y & 1) res = res * x;
      x = x * x;
      y >>= 1;
    }
    return res;
  }

  bool check(const int &tx, const int &ty, const int &n, const int &m) {
    if (0 <= tx && tx < n && 0 <= ty && ty < m) {
      return true;
    } else {
      return false;
    }
  }
}