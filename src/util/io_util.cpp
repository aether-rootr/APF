#include "util/io_util.h"

namespace IO {
  Inputer::Inputer() {

  }

  void Inputer::InputerFromFile(std::vector< std::vector < int > > &graph, const std::string &file_path) {
    std::ifstream file(file_path);
    graph.clear();

    std::string line;
    while (std::getline(file, line)) {
      std::vector <int> vec_line;
      for (auto ch : line) {
        if (ch == '0' || ch == '1') {
          vec_line.emplace_back(ch - '0');
        } else {
          throw "File data is illegal";
        }
      }
      graph.emplace_back(vec_line);
    }

    for (int i = 1; i < graph.size(); i ++) {
      if (graph[i].size() != graph[i].size()) {
        throw "File data is illegal";
      }
    }
  }
} // namespace IO
