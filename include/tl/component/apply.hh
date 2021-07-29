#pragma once

#include <cstddef>
#include <tuple>
#include <utility>

namespace tl::blacklist {

template <typename Func, typename Args, typename IndexList>
struct apply_helper;

template <typename Func, typename Tuple, size_t... I>
struct apply_helper<Func, Tuple, std::index_sequence<I...>> {
  static auto apply(Func&& func, Tuple args) {
    return func(std::get<I>(std::forward<Tuple>(args))...);
  }
};

template <typename Func, typename... T>
inline auto apply(Func&& func, std::tuple<T...>&& args) {
  using helper =
      apply_helper<Func, std::tuple<T...>&&, std::index_sequence_for<T...>>;
  return helper::apply(std::forward<Func>(func), std::move(args));
}

template <typename Func, typename... T>
inline auto apply(Func&& func, std::tuple<T...>& args) {
  using helper =
      apply_helper<Func, std::tuple<T...>&, std::index_sequence_for<T...>>;
  return helper::apply(std::forward<Func>(func), args);
}

template <typename Func, typename... T>
inline auto apply(Func&& func, const std::tuple<T...>& args) {
  using helper = apply_helper<Func, const std::tuple<T...>&,
                              std::index_sequence_for<T...>>;
  return helper::apply(std::forward<Func>(func), args);
}
template <typename Func>
inline auto apply(Func&& func) {
  if constexpr (std::is_same_v<void, std::invoke_result_t<Func>>) {
    func();
  } else {
    return func();
  }
}

}  // namespace tl::blacklist
