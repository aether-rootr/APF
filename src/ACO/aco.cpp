/**
 * 蚁群算法的实现
 */
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

  void ACO::init(const std::vector <std::vector <int>> &_map) {
    map = std::make_unique<Map>(_map);
    add.resize(map -> get_col());
    phe.resize(map -> get_col());
    for (int i = 0; i < map -> get_col(); i++) {
      add[i].resize(map -> get_row());
      phe[i].resize(map -> get_row());
    }
  }

  bool ACO::find_path(const Point &start, const Point &end) {
    int N1 = map -> get_col(), N2 = map -> get_row();

    for (int i = 0; i < N1; i++) {
      for (int j = 0; j < N2; j++) {
        phe[i][j] = IN;
        map->reset_vis(i, j);
      }
    }

    std::stack<Point> stackpath[M];
    Map Ini_map[M];
    Point AntPos[M];

    // 下面这个部分是用于获取cpu核心数以用于实现线程池的
    int cpu_count = 1;
#ifdef WINDOWS
  SYSTEM_INFO sysInfo;
  GetSystemInfo(&sysInfo);
  cpu_count = sysInfo.dwNumberOfProcessors;
#elif __linux__
  cpu_count = sysconf(_SC_NPROCESSORS_ONLN);
#endif

    // 线程池
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