#ifndef POINT_H
#define POINT_H

namespace ACO {
  struct Point {
    int x;
    int y;
    
    explicit Point() {
      
    }

    explicit Point(const int &x, const int &y) {
      this -> x = x;
      this -> y = y;
    }

    void operator =(const Point &Y) {
      this -> x = Y.x;
      this -> y = Y.y;
    }
  };

  double get_dist(const Point &a, const Point &b);
};

#endif