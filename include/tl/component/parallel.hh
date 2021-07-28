#pragma once
#define _PARALLEL_HPP
#include <functional>
#include <future>
#include <thread>
namespace tl::blacklist{

namespace detail
{
    template<typename L, typename R>
    struct has_operator_minus_impl
    {
        template<typename T = L, typename U = R> // template parameters here to enable SFINAE
        static auto test(T &&t, U &&u) -> decltype(t - u, void(), std::true_type{});
        static auto test(...) -> std::false_type;
        using type = decltype(test(std::declval<L>(), std::declval<R>()));
    };

    template<typename L, typename R = L>
    struct has_operator_minus : detail::has_operator_minus_impl<L, R>::type {};

    template<typename L, typename R = L>
    inline constexpr bool has_operator_minus_v = has_operator_minus<L, R>::type::value;

    template<typename L>
    struct has_operator_plus_prefix_impl
    {
        template<typename T = L> // template parameters here to enable SFINAE
        static auto test(T &&t) -> decltype(++t, void(), std::true_type{});
        static auto test(...) -> std::false_type;
        using type = decltype(test(std::declval<L>()));
    };

    template<typename L>
    struct has_operator_plus_prefix : detail::has_operator_plus_prefix_impl<L>::type {};

    template<typename L>
    inline constexpr bool has_operator_plus_prefix_v = has_operator_plus_prefix<L>::type::value;
} // namespace detail

} // namspace tl::blacklist
