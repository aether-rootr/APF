/**
 * 这里实现了一个线程池，使用方法参照 https://github.com/progschj/ThreadPool
 * 关于线程池:
 * https://en.wikipedia.org/wiki/Thread_pool
 * https://zh.wikipedia.org/wiki/%E7%BA%BF%E7%A8%8B%E6%B1%A0
 * https://baike.baidu.com/item/%E7%BA%BF%E7%A8%8B%E6%B1%A0/4745661?fr=aladdin
 * 这里多说几句，这里的线程池是基于cpp的一种回调机制实现的，所以跟异步函数的使用是类似的
 */
#ifndef THREAD_POOL_H
#define THREAD_POOL_H

#include <vector>
#include <queue>
#include <memory>
#include <thread>
#include <mutex>
#include <condition_variable>
#include <future>
#include <functional>
#include <stdexcept>

namespace ThreadPool {
  class ThreadPool {
  public:
      ThreadPool(size_t);
      template<class F, class... Args>
      auto enqueue(F&& f, Args&&... args) 
          -> std::future<typename std::result_of<F(Args...)>::type>;
      ~ThreadPool();
  private:
      // need to keep track of threads so we can join them
      std::vector< std::thread > workers;
      // the task queue
      std::queue< std::function<void()> > tasks;
      
      // synchronization
      std::mutex queue_mutex;
      std::condition_variable condition;
      bool stop;
  };
  
  // the constructor just launches some amount of workers
  inline ThreadPool::ThreadPool(size_t threads)
      :   stop(false)
  {
      for(size_t i = 0;i<threads;++i)
          workers.emplace_back(
              [this]
              {
                  for(;;)
                  {
                      std::function<void()> task;

                      {
                          std::unique_lock<std::mutex> lock(this->queue_mutex);
                          this->condition.wait(lock,
                              [this]{ return this->stop || !this->tasks.empty(); });
                          if(this->stop && this->tasks.empty())
                              return;
                          task = std::move(this->tasks.front());
                          this->tasks.pop();
                      }

                      task();
                  }
              }
          );
  }

  // add new work item to the pool
  template<class F, class... Args>
  auto ThreadPool::enqueue(F&& f, Args&&... args) 
      -> std::future<typename std::result_of<F(Args...)>::type>
  {
      using return_type = typename std::result_of<F(Args...)>::type;

      auto task = std::make_shared< std::packaged_task<return_type()> >(
              std::bind(std::forward<F>(f), std::forward<Args>(args)...)
          );
          
      std::future<return_type> res = task->get_future();
      {
          std::unique_lock<std::mutex> lock(queue_mutex);

          // don't allow enqueueing after stopping the pool
          if(stop)
              throw std::runtime_error("enqueue on stopped ThreadPool");

          tasks.emplace([task](){ (*task)(); });
      }
      condition.notify_one();
      return res;
  }

  // the destructor joins all threads
  inline ThreadPool::~ThreadPool()
  {
      {
          std::unique_lock<std::mutex> lock(queue_mutex);
          stop = true;
      }
      condition.notify_all();
      for(std::thread &worker: workers)
          worker.join();
  }
};

#endif