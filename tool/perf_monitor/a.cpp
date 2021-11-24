#include <iostream>
#include <cstdio>
#include <unistd.h>
#include <chrono>

int main() {
  auto start = std::chrono::system_clock::now();
  std::cout << "this is child process." << std::endl;
  sleep(1);
  auto end = std::chrono::system_clock::now();
  auto duration = std::chrono::duration_cast<std::chrono::microseconds>(end - start);
  std::cout <<  "花费了1: " 
     << double(duration.count()) * std::chrono::microseconds::period::num / std::chrono::microseconds::period::den 
     << "秒" << std::endl;
}