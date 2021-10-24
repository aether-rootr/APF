#include <iostream>
#include <vector>
#include <string>
#include <fstream>
#include <exception>

namespace ACO {
  class Inputer {
    public:
      explicit Inputer();

      void InputerFromFile(std::vector< std::vector < int > > &graph, const std::string &file_path);
  };

  class Outputer {

  };
};