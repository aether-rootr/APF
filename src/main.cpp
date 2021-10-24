#include <vector>
#include <iostream>
#include "ACO/aco.h"
#include "ACO/point.h"
#include "ACO/map.h"

int main() {
  std::vector < std::vector <int> > graph;

  graph.resize(8);
  for (int i = 0; i < 8; i ++) {
    graph[i].resize(8);
  }

  for (int i = 0; i < 8; i ++) {
    for (int j = 0; j < 8; j ++) {
      graph[i][j] = 0;
    }
  }

  graph[0][2] = 1;graph[0][6] = 1;graph[1][2] = 1;
  graph[2][4] = 1;graph[2][5] = 1;graph[3][1] = 1;graph[3][2] = 1;
  graph[3][3] = 1;graph[4][3] = 1;graph[5][1] = 1;graph[5][5] = 1;
  graph[6][1] = 1;graph[6][2] = 1;graph[6][3] = 1;graph[6][5] = 1;
  graph[7][0] = 1;graph[4][1] = 1;

  for (int i = 0; i < 8; i ++) {
    for (int j = 0; j < 8; j++) {
      std::cout << graph[i][j] << " ";
    }std::cout << std::endl;
  }

  ACO::ACO aco(10, 4000, 1, 2, 2, 0.7, 10);
  ACO::Point start, end;
  start.x = 0, start.y = 0;
  end.x = 7, end.y = 7;
  aco.init(graph, end);
  bool flag = aco.find_path(start, end);

  if (flag) {
    std::cout << aco.get_ans() << std::endl;
    auto path = aco.get_ans_path();
    while (!path.empty()) {
      graph[path.top().x][path.top().y] = 2;
      path.pop();
    }
    
    for (int i = 0; i < 8; i ++) {
      for (int j = 0; j < 8; j++) {
        std::cout << graph[i][j] << " ";
      }std::cout << std::endl;
    }
  } else {
    std::cout << "Path does not exist" << std::endl;
  }
}