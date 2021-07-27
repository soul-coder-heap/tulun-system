#pragma once
/*
 *from seastar https://sourcegraph.com/tithub.com/
 *soul-coder-heap/seastar/-/blob/include/seastar/core/function_traits.hh
 */
#include <tuple>
#include "tl/component/logger.h"
namespace tl {
namespace blacklist {

template <typename T>
class function_traits;

template <typename Ret, typename... Args>
class function_traits<Ret(Args...)> {
 public:
  function_traits(){
    LOG_INFO("base class function_traits<Ret(Args...)>"); 
  }
  using return_type = Ret;
  using args_as_tuple = std::tuple<Args...>;
  using signature = Ret(Args...);
  static constexpr std::size_t arity = sizeof...(Args);
  template <std::size_t N>
  using get_type = typename std::tuple_element<N, std::tuple<Args...>>::type;
};

// clang-format off
template <typename Ret, typename... Args>
class function_traits<Ret (*)(Args...)> : public function_traits<Ret(Args...)> {
public:
    function_traits(){
        LOG_INFO("class function_traits<Ret (*)(Args...)> : public function_traits<Ret(Args...)>");
    }
};

template <typename T, typename Ret, typename... Args>
class function_traits<Ret (T::*)(Args...)>
    : public function_traits<Ret(Args...)> {
public:
    function_traits(){
        LOG_INFO("class function_traits<Ret (T::*)(Args...)> : public function_traits<Ret(Args...)>");
    }
};

template <typename T, typename Ret, typename... Args>
class function_traits<Ret (T::*)(Args...) const>
    : public function_traits<Ret(Args...)> {
public:
    function_traits(){
        LOG_INFO("class function_traits<Ret (T::*)(Args...) const> : public function_traits<Ret(Args...)>");
    }

};

template <typename T>
class function_traits : public function_traits<decltype(&T::operator())> {
public:
    function_traits(){
        LOG_INFO("class function_traits : public function_traits<decltype(&T::operator())>");
    }
};

template <typename T>
class function_traits<T&> : public function_traits<std::remove_reference<T>> {
public:
    function_traits(){
        LOG_INFO("class function_traits<T&> : public function_traits<std::remove_reference<T>>");
    }
    
};
// clang-format on

}  // namespace blacklist
}  // namespace tl
