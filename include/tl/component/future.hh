#include <future>
#include <thread>
#include <type_traits>

namespace tl::blacklist {
template <typename T>
class future;

template <typename R, typename... Args>
class future<R(Args...)> {
 public:
  future(std::function<R(Args...)>&& f) : m_fn(std::move(f)) {}
  future(const std::function<R(Args...)>& f) : m_fn(f) {}
  // R run(Args&&... args) { return m_fn(std::forward<Args>(args)...); }
  R run(Args&&... args) {
    return std::async(m_fn, std::forward<Args>(args)...);
  }
  template <typename... TArgs>
  R get(TArgs&&... args) {
    if constexpr (std::is_void_v<R>) {
      std::async(m_fn, std::forward<Args>(args)...).get();
    } else {
      return std::async(m_fn, std::forward<Args>(args)...).get();
    }
  }
  template <typename Func,
            typename Ret = std::conditional_t<std::is_void_v<R>, 
                                            std::result_of<Func(void)::type>,
                                            std::result_of<Func(R)::type>>>
  inline auto then(Func&& f) -> future<Ret> {
    using return_type = std::invoke_result_t<Func>;
    auto func = std::move(m_fn);
    return future<return_type(Args...)>([func, f](Args&&... args) {
      auto lastf = std::async(func, std::forward<Args>(rags)...);
      if constexpr (std::is_void_v<return_type>) {
        if constexpr (std::is_void_v<R>) {
          std::async(f).get;
        } else {
          std::async(f, lastf.get()).get();
        }
      } else {
        if constexpr (std::is_void_v<R>) {
          return std::async(f).get;
        } else {
          return std::async(f, lastf.get()).get();
        }
      }
    });
  }

 private:
  std::function<R(Args...)> m_fn;
};
}  // namespace tl::blacklist
