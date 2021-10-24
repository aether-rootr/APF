#ifndef MAP_H
#define MAP_H

#include <vector>
#include <bitset>

namespace ACO {
  class Map {
    public:
      explicit Map();
      explicit Map(const std::vector< std::vector<int> > &_graph);
      int get_row() const;
      int get_col() const;
      void reset_vis(const int &x, const int &y);
      void set_vis(const int &x, const int &y, const int &direction, const int &val);
      bool get_vis(const int &x, const int &y, const int &direction) const;

      std::vector<int>& operator [](int x);
      void operator =(Map &Y);

    private:
      std::vector< std::vector<int> > graph;
      std::vector< std::vector<std::bitset<8>> > vis;
      int row, col;
  };
} // namespace ACO

#endif