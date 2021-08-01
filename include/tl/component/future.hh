#include <future>
#include <thread>
#include <type_traits>
#include "tl/component/function_traits.hh"

namespace tl::blacklist {
/*
template <typename T>
std::enable_if_t<std::is_void_v<T>, T> simpleTypeInfo(T t) {
  return t;
}

template <typename T>
std::enable_if_t<not std::is_void_v<T>, T> simpleTypeInfo(T t) {
  return t;
}
template <class R, class... Args>
R getType(R(*)(Args...));
*/
template <typename T> class future;

template <typename R, typename... Args> class future<R(Args...)> {
public:
  future(std::function<R(Args...)> &&f) : m_fn(std::move(f)) {}
  future(const std::function<R(Args...)> &f) : m_fn(f) {}
  // R run(Args&&... args) { return m_fn(std::forward<Args>(args)...); }//sync
  R run(Args &&... args) {
    if constexpr (std::is_void_v<R>) {
      std::async(m_fn, std::forward<Args>(args)...).get();
    } else {
      return std::async(m_fn, std::forward<Args>(args)...).get();
    }
  }
  template <typename... TArgs> R get(TArgs &&... args) {
    if constexpr (std::is_void_v<R>) {
      std::async(m_fn, std::forward<TArgs>(args)...).get();
    } else {
      return std::async(m_fn, std::forward<TArgs>(args)...).get();
    }
  }
  template <
      typename Func /*, typename Ret = typename std::result_of<Func(R)>::type*/>
  inline auto then(Func &&f)
  /*-> future<
  // typename std::result_of<Func(R)>::type(Args...)
  std::conditional_t<std::is_void_v<int>,
  typename std::result_of<Func(void)>::type(Args...),
  typename std::result_of<Func(R)>::type(Args...)>> */
  {
    auto func = std::move(m_fn);
    //typedef typename std::result_of<Func(void)>::type TRet; // void type error
    //typedef typename std::result_of<Func(R)>::type TRet;  // return type error
    //typedef typename std::result_of<typename std::decay<decltype(f)>::type(R)>::type TRet;
    if constexpr (std::is_void_v<R>) {
      //using Ret = typename decltype(getType(f));
      //using Ret = typename function_traits<decltype(Func{})>::return_type;
      //using Ret = typename std::result_of<typename std::decay<std::decltype(f)>::type(int)>::type;
      //using Ret = typename std::result_of<std::decltype(f)*()>::type;
      //using Ret = typename std::invoke_result_t<decltype(f)>;
      //typedef typename std::result_of<typename std::decay<decltype(f)>::type()>::type Ret;
      using Ret = typename std::result_of<typename std::decay<decltype(f)>::type()>::type;
      return future<Ret(Args...)>([func, f](Args &&... args) {
        std::async(func, std::forward<Args>(args)...).get();
        if constexpr (std::is_void_v<Ret>) {
            std::async(f).get();
        } else {
            return std::async(f).get();
        }
      });
    } else {
      //using Ret = std::invoke_result_t<decltype(f)>;
      using Ret = typename std::result_of<Func(R)>::type;
      return future<Ret(Args...)>([func, f](Args &&... args) {
        auto lastf = std::async(func, std::forward<Args>(args)...);
        if constexpr (std::is_void_v<Ret>) {
            std::async(f, lastf.get()).get();
        } else {
            return std::async(f, lastf.get()).get();
        }
      });
    }
  }

private:
  std::function<R(Args...)> m_fn;
};
} // namespace tl::blacklist
