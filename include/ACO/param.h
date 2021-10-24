#ifndef PARAM_H
#define PARAM_H

namespace ACO {
  const double MAX =  0x7fffffff;
  const int dx[] = {-1, -1, -1, 0, 1, 1, 1, 0};
  const int dy[] = {-1, 0, 1, 1, 1, 0, -1, -1};
  const int update_phe[8][8] = {
                                {4, 3, 2, 1, 0, 1, 2, 3},
                                {3, 4, 3, 2, 1, 0, 1, 2},
                                {2, 3, 4, 3, 2, 1, 0, 1},
                                {1, 2, 3, 4, 3, 2, 1, 0},
                                {0, 1, 2, 3, 4, 3, 2, 1},
                                {1, 0, 1, 2, 3, 4, 3, 2},
                                {2, 1, 0, 1, 2, 3, 4, 3},
                                {3, 2, 1, 0, 1, 2, 3, 4}
                              };
} // namespace ACO

#endif