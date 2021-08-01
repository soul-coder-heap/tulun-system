#include <atomic>
#include <boost/range/irange.hpp>
#include <chrono>
#include <condition_variable>
#include <iostream>
#include <thread>
#include <vector>
#include <boost/range/irange.hpp>
#include "tl/component/threadpool.h"

using namespace std::chrono;
using namespace std::chrono_literals;

long SerialFib(long n) noexcept {
  if (n < 2)
    return n;
  else
    return SerialFib(n - 1) + SerialFib(n - 2);
}

double detect_serial_time(long n) {
  const auto start = system_clock::now();
  long result = 0L;
  int c = 100;
  for (int i = 0; i < c; ++i) {
    result += SerialFib(n);
  }
  const auto end = system_clock::now();
  auto duration = duration_cast<microseconds>(end - start);
  std::cout << "single serial took: " << (double)duration.count() / c
            << "us, result=" << result << std::endl;
  return (double)duration.count() / c;
}

int main(int argc, char **argv) {
  int n = 15;
  int t = 8;
  auto single_rt = detect_serial_time(n);
  tl::blacklist::ThreadPool tp(t);
  std::atomic<int> count{0};
  auto start = system_clock::now();
  int total = 8000;
  std::condition_variable cv;
  std::mutex m;

  for (int i = 0; i < total; ++i) {
    tp.Submit([n] { SerialFib(n); },
              [&]() mutable {
                ++count;
                if (count >= total) {
                  const auto end = system_clock::now();
                  auto duration = duration_cast<microseconds>(end - start);

                  std::cout << "total runtime took: " << duration.count()
                            << "us" << std::endl;
                  std::cout << "parallel speedup ratio with " << t
                            << " threads: "
                            << (double)(single_rt * total) / duration.count()
                            << std::endl;
                  cv.notify_all();
                }
              });
  }
  {
    std::unique_lock<std::mutex> lk(m);
    cv.wait(lk);
  }

  {
    start = system_clock::now();
    auto range = boost::irange<int>(0, total);

    std::atomic_int64_t sum{0L};
    tp.ForEachRange(
        std::begin(range), std::end(range),
        [n, &sum](auto iter) noexcept {
          int i = *iter;
          sum += i;
          SerialFib(n);
        },
        [&cv]() { cv.notify_all(); });
    std::unique_lock<std::mutex> lk(m);
    cv.wait(lk);
    const auto end = system_clock::now();
    auto duration = duration_cast<microseconds>(end - start);
    std::cout << "total foreach runtime took: " << duration.count()
              << "us, sum = " << sum << std::endl;
    std::cout << "parallel foreach speedup ratio with " << t
              << " threads: " << (double)(single_rt * total) / duration.count()
              << std::endl;
  }

  // chunk task
  {
    start = system_clock::now();
    int tasknum = t;
    int subrange = total / tasknum;

    auto range = boost::irange<int>(0, tasknum);
    std::atomic_int64_t sum{0L};
    tp.ForEachRange(
        std::begin(range), std::end(range),
        [n, subrange, &sum](auto iter) noexcept {
          sum += 1;
          const auto start = system_clock::now();
          for (int i = 0; i < subrange; ++i) {
            SerialFib(n);
          }
          const auto end = system_clock::now();
          auto duration = duration_cast<microseconds>(end - start);

          std::cout << "thread: " << *iter << " finished in "
                    << duration.count() << "us\n";
        },
        [&cv]() { cv.notify_all(); });
    std::unique_lock<std::mutex> lk(m);
    cv.wait(lk);
    const auto end = system_clock::now();
    auto duration = duration_cast<microseconds>(end - start);
    
    std::cout << "total batch foreach runtime took: " << duration.count()
              << "us, sum: " << sum << std::endl;
    std::cout << "parallel batch foreach speedup ratio with " << t
              << " threads: " << (double)(single_rt * total) / duration.count()
              << std::endl;
  }
  return 0;
}
