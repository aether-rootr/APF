/**
 * 这个文件定义了一个地图类
 */
#ifndef MAP_H
#define MAP_H

#include <vector>
#include <bitset>

namespace ACO {
  class Map {
    public:
      explicit Map();
      explicit Map(const std::vector< std::vector<int> > &_graph);
      int get_row() const; // 获取行数
      int get_col() const;// 获取列数
      void reset_vis(const int &x, const int &y);// 清空vis数组
      void set_vis(const int &x, const int &y, const int &direction, const int &val);// 设置某个方向的vis值
      bool get_vis(const int &x, const int &y, const int &direction) const; // 获取某个方向的vis值

      std::vector<int>& operator [](int x);
      void operator =(Map &Y);

    private:
      std::vector< std::vector<int> > graph;
      std::vector< std::vector<std::bitset<8>> > vis;
      int row, col;
  };
} // namespace ACO

#endif