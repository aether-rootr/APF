/**
 * 这个文件主要定义了一些算法运行过程中用到的一些固定参数
 * 例如用于遍历的dx和dy数组是用于遍历可以转移方向使用的
 */

#ifndef PARAM_H
#define PARAM_H

namespace ACO {
  const double MAX =  0x7fffffff;
  const int dx[] = {-1, -1, -1, 0, 1, 1, 1, 0};
  const int dy[] = {-1, 0, 1, 1, 1, 0, -1, -1};
} // namespace ACO

#endif