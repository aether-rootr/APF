#include <math.h>
#include <thread>
#include <vector>
#ifdef WINDOWS
  #include "windows.h"
#elif __linux__
  #include "unistd.h"
#endif

#include "ACO/aco.h"
#include "ACO/param.h"

#include "../util/util.h"
#include "../util/thread_pool.h"

namespace ACO {
  ACO::ACO(const int &_M, const int &_RcMax, const double &_IN, const double &_alphe, const double &_beta, const double &_rout, const double &_Q) :
  M(_M), RcMax(_RcMax), IN(_IN), alphe(_alphe), beta(_beta), rout(_rout), Q(_Q) {
    ans = MAX;
  }

  void ACO::init(const std::vector <std::vector <int>> &_map, const Point &end) {
    map = std::make_unique<Map>(_map);
    add.resize(map -> get_col());
    phe.resize(map -> get_col());
    for (int i = 0; i < map -> get_col(); i++) {
      add[i].resize(map -> get_row());
      phe[i].resize(map -> get_row());
    }

    int N1 = map -> get_col(), N2 = map -> get_row();
    double k = 0.9, eta = 0.9;
    int p0 = 5;

    double force[N1][N2][4];
    for (int i = 0; i < N1; i++) {
      for (int j = 0; j < N2; j++) {
        force[i][j][0] = 0.0;
        force[i][j][1] = 0.0;
        force[i][j][2] = 0.0;
        force[i][j][3] = 0.0;
      }
    }

    for (int i = 0; i < N1; i++) {
      for (int j = 0; j < N2; j++) {
        if ((*map)[i][j] != 1) {
          int x0 = i, y0 = j, x1, y1;
          for (int x1 = std::max(x0 - p0, 0); x1 < std::min(N1, x0 + p0 + 1); x1 ++) {
            for (int y1 = std::max(x0 - p0, 0); y1 < std::min(N1, x0 + p0 + 1); y1 ++) {
              if ((*map)[x1][y1] != 1) continue;

              double base = (1.0 / get_dist(Point(x0, y0), Point(x1, y1)) - 1.0 / p0);
              double U = eta * base * base / 2;
              
              double cos_x , cos_y;
              if (x0 - x1 != 0) {
                cos_x = get_dist(Point(x0, y0), Point(x1, y1)) / (x0 - x1);
              } else {
                cos_x = 0;
              }
              if (y0 - y1 != 0) {
                cos_y = get_dist(Point(x0, y0), Point(x1, y1)) / (y0 - y1);
              } else {
                cos_y = 0;
              }

              double force_x = U * cos_x;
              double force_y = U * cos_y;

              if (force_x < 0) {
                force[x0][y0][1] += force_x;
              } else {
                force[x0][y0][3] += force_x;
              }
              if (force_y < 0) {
                force[x0][y0][0] += force_y;
              } else {
                force[x0][y0][2] += force_y;
              }
            }
          }

          double base = (1.0 / get_dist(Point(x0, y0), Point(end.x, end.y)) - 1.0 / p0);
          double U = eta * base * base / 2;
          
          double cos_x , cos_y;
          if (x0 - x1 != 0) {
            cos_x = get_dist(Point(x0, y0), Point(end.x, end.y)) / (x0 - end.x);
          } else {
            cos_x = 0;
          }
          if (y0 - y1 != 0) {
            cos_y = get_dist(Point(x0, y0), Point(end.x, end.y)) / (y0 - end.y);
          } else {
            cos_y = 0;
          }

          double force_x = U * cos_x;
          double force_y = U * cos_y;

          if (force_x >= 0) {
            force[x0][y0][1] += force_x;
          } else {
            force[x0][y0][3] += force_x;
          }
          if (force_y >= 0) {
            force[x0][y0][0] += force_y;
          } else {
            force[x0][y0][2] += force_y;
          }

          force_x = force[x0][y0][1] - force[x0][y0][3];
          force_y = force[x0][y0][0] - force[x0][y0][2];
          double h = sqrt(force_x * force_x + force_y * force_y);
          cos_x = h / abs(force_x);
          cos_y = h / abs(force_y);
          double cos_mid = sqrt(2) / 2;
          double cos_to_mid = cos_mid * cos_x + cos_mid * cos_y;
          
          /**
           * 这里在实现的时候通过复杂的判断后，通过循环提前编写好的权值矩阵来给信息素赋值
           * 减少了编码量，并且可以进一步的权值表来降低代码复杂性
           */

          if (force_x >= 0 && force_y >= 0) {
            if (cos_to_mid >= cos_x && cos_to_mid >= cos_y) {
              for (int op = 0; op < 8; op ++) {
                int tx = x0 + dx[op];
                int ty = y0 + dy[op];
                if (!check(tx, ty, N1, N2)) {
                  continue;
                }
                phe[tx][ty] += update_phe[2][op];
              }
            } else if (cos_x > cos_y) {
              for (int op = 0; op < 8; op ++) {
                int tx = x0 + dx[op];
                int ty = y0 + dy[op];
                if (!check(tx, ty, N1, N2)) {
                  continue;
                }
                phe[tx][ty] += update_phe[3][op];
              }
            } else {
              for (int op = 0; op < 8; op ++) {
                int tx = x0 + dx[op];
                int ty = y0 + dy[op];
                if (!check(tx, ty, N1, N2)) {
                  continue;
                }
                phe[tx][ty] += update_phe[1][op];
              }
            }
          } else if (force_x < 0 && force_y >= 0) {
            if (cos_to_mid >= cos_x && cos_to_mid >= cos_y) {
              for (int op = 0; op < 8; op ++) {
                int tx = x0 + dx[op];
                int ty = y0 + dy[op];
                if (!check(tx, ty, N1, N2)) {
                  continue;
                }
                phe[tx][ty] += update_phe[0][op];
              }
            } else if (cos_x > cos_y) {
              for (int op = 0; op < 8; op ++) {
                int tx = x0 + dx[op];
                int ty = y0 + dy[op];
                if (!check(tx, ty, N1, N2)) {
                  continue;
                }
                phe[tx][ty] += update_phe[7][op];
              }
            } else {
              for (int op = 0; op < 8; op ++) {
                int tx = x0 + dx[op];
                int ty = y0 + dy[op];
                if (!check(tx, ty, N1, N2)) {
                  continue;
                }
                phe[tx][ty] += update_phe[1][op];
              }
            }
          } else if (force_x >= 0 && force_y < 0) {
            if (cos_to_mid >= cos_x && cos_to_mid >= cos_y) {
              for (int op = 0; op < 8; op ++) {
                int tx = x0 + dx[op];
                int ty = y0 + dy[op];
                if (!check(tx, ty, N1, N2)) {
                  continue;
                }
                phe[tx][ty] += update_phe[4][op];
              }
            } else if (cos_x > cos_y) {
              for (int op = 0; op < 8; op ++) {
                int tx = x0 + dx[op];
                int ty = y0 + dy[op];
                if (!check(tx, ty, N1, N2)) {
                  continue;
                }
                phe[tx][ty] += update_phe[3][op];
              }
            } else {
              for (int op = 0; op < 8; op ++) {
                int tx = x0 + dx[op];
                int ty = y0 + dy[op];
                if (!check(tx, ty, N1, N2)) {
                  continue;
                }
                phe[tx][ty] += update_phe[5][op];
              }
            }
          } else {
            if (cos_to_mid >= cos_x && cos_to_mid >= cos_y) {
              for (int op = 0; op < 8; op ++) {
                int tx = x0 + dx[op];
                int ty = y0 + dy[op];
                if (!check(tx, ty, N1, N2)) {
                  continue;
                }
                phe[tx][ty] += update_phe[6][op];
              }
            } else if (cos_x > cos_y) {
              for (int op = 0; op < 8; op ++) {
                int tx = x0 + dx[op];
                int ty = y0 + dy[op];
                if (!check(tx, ty, N1, N2)) {
                  continue;
                }
                phe[tx][ty] += update_phe[7][op];
              }
            } else {
              for (int op = 0; op < 8; op ++) {
                int tx = x0 + dx[op];
                int ty = y0 + dy[op];
                if (!check(tx, ty, N1, N2)) {
                  continue;
                }
                phe[tx][ty] += update_phe[5][op];
              }
            }
          }
        }
        map->reset_vis(i, j);
      }
    }
  }

  bool ACO::find_path(const Point &start, const Point &end) {
    int N1 = map -> get_col(), N2 = map -> get_row();

    std::stack<Point> stackpath[M];
    Map Ini_map[M];
    Point AntPos[M];

    int cpu_count = 1;
#ifdef WINDOWS
  SYSTEM_INFO sysInfo;
  GetSystemInfo(&sysInfo);
  cpu_count = sysInfo.dwNumberOfProcessors;
#elif __linux__
  cpu_count = sysconf(_SC_NPROCESSORS_ONLN);
#endif

    ThreadPool::ThreadPool pool(cpu_count);

    int round = 0;
    while (round < RcMax) {
      std::vector< std::future<void> > round_init;

      for (int i = 0; i < M; i ++) {
        round_init.emplace_back(
          pool.enqueue([&](int id) {
            while (!stackpath[id].empty()) {
              stackpath[id].pop();
            }
            Ini_map[id] = *map;
            Ini_map[id][start.x][start.y] = 1;
            stackpath[id].push(start);
            AntPos[id] = start;
          }, i)
        );
      }

      std::vector< std::future<bool> > round_work;

      for (int i = 0; i < M; i++) {
        round_init[i].get();
        round_work.emplace_back(
          pool.enqueue([&](int id) {
            while ((AntPos[id].x != end.x) || (AntPos[id].y != end.y)) {
              double psum = 0;
              for (int x = 0; x < 8; x++) {
                int tx = AntPos[id].x + dx[x];
                int ty = AntPos[id].y + dy[x];
                if (!check(tx, ty, N1, N2)) {
                  continue;
                }
                if (dx[x] != 0 && dy[x] != 0) {
                  if (Ini_map[id][tx][AntPos[id].y] ==1 && Ini_map[id][AntPos[id].x][ty] == 1) {
                    continue;
                  }
                }
                if (Ini_map[id].get_vis(AntPos[id].x, AntPos[id].y, x) == 0 && Ini_map[id][tx][ty] != 1) {
                  psum += pow(phe[tx][ty], alphe) * pow((10.0/stackpath[id].size()), beta);
                }
              }

              if (psum != 0) {
                double drand = (double)(rand()) / RAND_MAX;
                double pro = 0;
                int tx, ty;
                for (int x = 0; x < 8; x++) {
                  tx = AntPos[id].x + dx[x];
                  ty = AntPos[id].y + dy[x];
                  if (!check(tx, ty, N1, N2)) {
                    continue;
                  }
                  if (dx[x] != 0 && dy[x] != 0) {
                    if (Ini_map[id][tx][AntPos[id].y] ==1 && Ini_map[id][AntPos[id].x][ty] == 1) {
                      continue;
                    }
                  }
                  if (Ini_map[id].get_vis(AntPos[id].x, AntPos[id].y, x) == 0 && Ini_map[id][tx][ty] != 1) {
                    pro += pow(phe[tx][ty], alphe) * pow((10.0/stackpath[id].size()), beta) / psum;
                    if (pro >= drand) {
                      break;
                    }
                  }
                }

                AntPos[id].x = tx;
                AntPos[id].y = ty;
                stackpath[id].push(AntPos[id]);
                Ini_map[id][AntPos[id].x][AntPos[id].y] = 1;
              } else {
                stackpath[id].pop();
                Ini_map[id][AntPos[id].x][AntPos[id].y] = 0;
                if (stackpath[id].empty()) {
                  return false;
                }

                int x1 = AntPos[id].x, y1 = AntPos[id].y, x2 = stackpath[id].top().x, y2 = stackpath[id].top().y;
                if (x1 - x2 == -1 && y1 - y2 == -1) {
                   Ini_map[id].set_vis(x2, y2, 0, 1);
                } else if (x1 - x2 == -1 && y1 - y2 == 0) {
                   Ini_map[id].set_vis(x2, y2, 1, 1);
                } else if (x1 - x2 == -1 && y1 - y2 == 1) {
                   Ini_map[id].set_vis(x2, y2, 2, 1);
                } else if (x1 - x2 == 0 && y1 - y2 == 1) {
                   Ini_map[id].set_vis(x2, y2, 3, 1);
                } else if (x1 - x2 == 1 && y1 - y2 == 1) {
                   Ini_map[id].set_vis(x2, y2, 4, 1);
                } else if (x1 - x2 == 1 && y1 - y2 == 0) {
                   Ini_map[id].set_vis(x2, y2, 5, 1);
                } else if (x1 - x2 == 1 && y1 - y2 == -1) {
                   Ini_map[id].set_vis(x2, y2, 6, 1);
                } else if (x1 - x2 == 0 && y1 - y2 == -1) {
                   Ini_map[id].set_vis(x2, y2, 7, 1);
                }
                AntPos[id].x = x2;
                AntPos[id].y = y2;
              }
            }
            return true;
          }, i)
        );
      }

      for (int i = 0; i < M; i ++) {
        bool path_exits = round_work[i].get();
        if (path_exits == false) {
          return false;
        }
        if (stackpath[i].size() < ans) {
          ans_path = stackpath[i];
          ans = stackpath[i].size();
        }
      }

      for (int i = 0; i < N1; i ++) {
        for (int j = 0; j < N2; j ++) {
          add[i][j] = 0;
        }
      }

      for (int i = 0; i < M; i ++) {
        double L = stackpath[i].size();
        double d = 1.0 * Q / L;
        while (!stackpath[i].empty()) {
          add[stackpath[i].top().x][stackpath[i].top().y] += d;
          stackpath[i].pop();
        }
      }

      for (int i = 0; i < N1; i ++) {
        for (int j = 0; j < N2; j ++) {
          phe[i][j] = phe[i][j] * (1 - rout) + add[i][j];
          if (phe[i][j]  <= 0.000001) {
            phe[i][j]  = 0.000001;
          }
        }
      }
      round ++;
    }
    return true;
  }

  double ACO::get_ans() const {
    return ans;
  }

  std::stack<Point> ACO::get_ans_path() const {
    return ans_path;
  }
} // namespace ACO