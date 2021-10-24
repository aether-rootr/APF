#include "ACO/map.h"

namespace ACO {
  Map::Map() {

  }

  Map::Map(const std::vector< std::vector<int> > &_graph) {
    row = _graph.size();
    if (row != 0) {
      col = _graph[0].size();
      vis.resize(row);
      graph.resize(row);
      for (int i = 0; i < row; i ++) {
        vis[i].resize(col);
        graph[i].resize(col);
        for (int j = 0; j < col; j ++) {
          graph[i][j] = _graph[i][j];
        }
      }
    } else {
      col = 0;
    }
  }

  int Map::get_col() const {
    return col;
  }
  int Map::get_row() const {
    return row;
  }

  void Map::reset_vis(const int &x, const int &y) {
    vis[x][y].reset();
  }

  void Map::set_vis(const int &x, const int &y, const int &direction, const int &val) {
    vis[x][y][direction] = val;
  }
  
  bool Map::get_vis(const int &x, const int &y, const int &direction) const {
    return vis[x][y][direction];
  }

  std::vector<int>& Map::operator [](int x) {
    return graph[x];
  }

  void Map::operator =(Map &Y) {
    this -> row = Y.get_row();
    this -> col = Y.get_col();
    this -> graph.resize(row);
    this -> vis.resize(row);
    for (int i = 0; i < row; i++) {
      this -> graph[i].resize(col);
      this -> vis[i].resize(col);
      for(int j = 0; j < col; j++) {
        this -> graph[i][j] = Y[i][j];
        this -> vis[i][j] = Y.vis[i][j];
      }
    }
  }
} // namespace ACO