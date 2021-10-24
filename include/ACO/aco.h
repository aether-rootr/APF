#ifndef ACO_H
#define ACO_H

#include <vector>
#include <set>
#include <stack>
#include <memory>

#include "ACO/point.h"
#include "ACO/map.h"

namespace ACO {
  class ACO {
    public:
      explicit ACO(const int &_M = 10, const int &_RcMax = 2000, const double &_IN = 1, const double &_alphe = 2, const double &_beta = 2, const double &_rout = 0.7, const double &_Q = 10);
      void init(const std::vector <std::vector <int> > &_map);
      bool find_path(const Point &start, const Point &end);
      double get_ans() const;
      std::stack<Point> get_ans_path() const;
    private:
      int M;
      int RcMax;
      double IN;
      std::vector< std::vector<double> > add;
      std::vector< std::vector<double> > phe;
      std::unique_ptr<Map> map;

      double alphe, beta, rout, Q, rou;

      double ans;
      std::stack<Point> ans_path;
  };
} // namespace ACO

#endif