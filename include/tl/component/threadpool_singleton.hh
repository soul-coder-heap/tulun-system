#pragma once
#include "tl/component/threadpool.h"
#include "tl/component/future.hh"
namespace tl::blacklist {
template <typename Func> struct CallbackTask : public Task {
  Func func_;
  CallbackTask(Func &&func) : func_(std::move(func)) {}
  virtual void operator()() override {func_();}
};
class ThreadpoolExecution {
private:
  static inline void callback(void *data) {
    std::unique_ptr<Task> p(reinterpret_cast<Task *>(data));
    (*p)();
  }
public:
  template <typename Func, typename Ret = std::invoke_result_t<Func>>
  static inline auto AsyncSubmit(ThreadPool *tp, Func &&func) {
    if constexpr (std::is_void_v<Ret>) {
      auto p = std::make_unique<std::promise<void>>();
      auto future = p->get_future();
      tp->Submit(std::move(func), [p = std::move(p)]() mutable {
        p->set_value();
      });
      return tl::blacklist::future<void()>([&future](){future.get();});
    } else {
      auto p = std::make_unique<std::promise<Ret>>();
      auto future = p->get_future();
      tp->Submit(std::move(func), [p = std::move(p)](Ret r) mutable {
        p->set_value(std::move(r));
      });
      return tl::blacklist::future<Ret()>([&future](){return future.get();});
    }
  }
};

class ThreadPoolSingleton {
public:
  static void Init(int n) {
    GetThreadPoolSingleton().reset(new tl::blacklist::ThreadPool(n));
  }

  static std::shared_ptr<tl::blacklist::ThreadPool> &GetThreadPoolSingleton() {
    static std::shared_ptr<tl::blacklist::ThreadPool> tp;
    return tp;
  }
  template <typename Func, typename Ret = std::invoke_result_t<Func>>
  static inline auto AsyncSubmit(Func &&func) {
    auto tp = GetThreadPoolSingleton();
    return ThreadpoolExecution::AsyncSubmit(tp.get(), std::move(func));
  }
};
} // namespace tl::blacklist
