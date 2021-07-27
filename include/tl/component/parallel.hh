#ifndef _PARALLEL_HPP
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

template <class T> 
class future;

template<class R, class...Args>
class future<R(Args...)>
{
private:
    std::function<R(Args...)> m_fn;
public:
    typedef R return_type;
    future(std::function<R(Args...)>&& f) :m_fn(std::move(f)) {}
    future(std::function<const R(Args...)>& f) :m_fn(f) {}
    ~future(){}
    // wait async task finish
    void wait(){
        std::async(m_fn).wait;
    }
    // get async task result
    template<typename... Args_>
    R get(Args&&... args){
        return std::async(m_fn, std::forward<Args_>(args)...).get();
    }
    //start
    std::shared_future<R> run(){
        return std::async(m_fn);
    }
    template<typename F>
    auto then(F&& f)->future<typename std::result_of<F(R)>::type(Args...)>
    {
        typedef typename std::result_of<F(R)>::type  ReturnType;
        auto func = std::move(m_fn);
        return future<ReturnType(Args...)>([func, &f](Args&& ...args){
            std::future<R> lastf = std::async(func, std::forward<Args>(args)...);
            return std::async(f, lastf.get()).get();
        });
    }
};
}
#endif
