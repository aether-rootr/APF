#ifndef POINT_H
#define POINT_H

namespace ACO {
  struct Point {
    int x;
    int y;
    
    void operator =(const Point &Y) {
      this -> x = Y.x;
      this -> y = Y.y;
    }
  };
};

#endif