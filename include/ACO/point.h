/**
 * 这个文件中定义了一个点结构体，用于描述一个二维的点
 */
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