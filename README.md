## Glossary

APF is Ant Colony Potential Field

ACO is Ant Colony Optimization

---

## How to use it

本项目使用了cmake进行编译，推荐使用docker
```bash
docker pull aethertaberu/apf
```

如何运行：

``` bash
cd APF
mkdir build
cd build
cmake ..
make
```

---

## 关于系统兼容问题

算法本体默认兼容Linux，虽然也做了一些关于window的兼容，但是不保证兼容windows。

tool/路径下的perf_monitor不支持windows。

---

## 关于分支

master分支实现了基本的蚁群算法

APFOA是利用引力势场法优化蚁群算法初始化的实现

APFGA是蚁群算法运行时结合引力势场法优化的实现

---

## 关于文件路径

文件路径：

include路径下包含本项目的头文件

src路径下包含本项目的源码实现

- ACO路径下主要实现了蚁群算法的本体
- util路径主要实现了一些算法中需要用到包括线程池在内的工具链

tool路径下是性能监测工具是实现。

另外paper路径下附上了论文，另外两个分支的做法皆是参考这篇论文实现的。

具体文件细节请参照注释。

---

## to do list

1. 接入util路径下的io_util实现从文件中读取数据

2. 实现迷宫数据生成器，自动化的生成测试数据
3. 优化其它分支的参数读取方式，在构造函数中设定参数，而不是在代码中定义值，通过修改源码来设定参数
4. 开发算法结果正确性评价工具
5. 适当补充一些注释
6. ......
