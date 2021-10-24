cmake_minimum_required(VERSION 3.5)
project(apf)

set(CMAKE_CXX_FLAGS   "-g")

set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_STANDARD 14)

include_directories("${PROJECT_SOURCE_DIR}/include")

file(GLOB_RECURSE SOURCE ./src/*.cpp)

add_executable(${PROJECT_NAME} ${SOURCE})
target_link_libraries(${PROJECT_NAME} 
  pthread
)