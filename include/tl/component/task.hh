#pragma once

#include <memory>
#include <type_traits>
#include <atomic>

namespace tl::blacklist {
class Task {
public:
  explicit Task() {}
  virtual ~Task() noexcept {}
  virtual void operator()() noexcept = 0;
};

template <typename Func, typename Result> class LambdaTask final : public Task {
  Func _func;
  Result _result;

public:
  LambdaTask(Func &&func, Result &&result)
      : _func(std::move(func)), _result(std::move(result)) {}
  virtual void operator()() noexcept override {
    if constexpr (std::is_same_v<void, std::invoke_result_t<Func>>) {
      _func();
      _result();
    } else {
      auto r = _func();
      _result(r);
    }
  }
};

struct TaskIterator
    : public std::enable_shared_from_this<TaskIterator> {
  virtual Task *operator*() = 0;
};

template <typename Func, typename Result>
static inline Task *MakeTask(Func &&func, Result &&result) {
  return new LambdaTask<Func, Result>(std::forward<Func>(func),
                                      std::forward<Result>(result));
}

template <typename Func, typename Result>
static inline std::unique_ptr<Task> MakeUniqueTask(Func &&func,
                                                   Result &&result) {
  return std::make_unique<LambdaTask<Func, Result>>(
      std::forward<Func>(func), std::forward<Result>(result));
}

template <typename It, typename Func, typename ResultFunc>
struct LambdaTaskIterator : public TaskIterator {
  It iter;
  Func func;
  ResultFunc resultfn;
  size_t total;
  std::atomic_int32_t count{0};

  LambdaTaskIterator(It &&iter, Func &&func, ResultFunc &&resultfn, size_t total)
      : iter(std::forward<It>(iter)), func(std::forward<Func>(func)),
        resultfn(std::forward<ResultFunc>(resultfn)), total(total) {}

  Task *operator*() override {
    using LambdaTaskIteratorPtr = std::shared_ptr<LambdaTaskIterator>;
    LambdaTaskIteratorPtr self =
        std::static_pointer_cast<LambdaTaskIterator>(shared_from_this());

    auto tf = [iter = iter, self] { self->func(iter); };
    auto rf = [self]() mutable {
      if (self->count < self->total) {
          self->resultfn();
      }
    };
    ++iter;
    return MakeTask(std::move(tf), std::move(rf));
  }
};


struct LambdaTaskIteratorWrapper {
  std::shared_ptr<TaskIterator> iter;

  LambdaTaskIteratorWrapper &operator++() { return *this; }

  LambdaTaskIteratorWrapper operator++(int) {
    LambdaTaskIteratorWrapper tmp = *this;
    return tmp;
  }

  Task *operator*() const { return *(*iter); }
};


} // namespace tl::blacklist
