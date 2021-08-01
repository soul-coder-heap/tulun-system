#pragma once
#include <functional>
#include <future>
#include <thread>
#include "tl/component/task.hh"
#include "tl/component/logger.h"
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

//pmpi
class ThreadPoolInternal;

class ThreadPool {
public:
    explicit ThreadPool(int nThreads);
    ~ThreadPool();
    ThreadPool(const ThreadPool& other) = delete;
    ThreadPool& operator=(const ThreadPool& other) = delete;
    template<typename TaskFunc, typename ResultFunc>
    void Submit(TaskFunc&& taskfn, ResultFunc&& resultfn){
        auto task = MakeTask(std::forward<TaskFunc>(taskfn), std::forward<ResultFunc>(resultfn));
        Submit(task);
    }

    template<typename TaskFunc, typename ResultFunc>
    void SubmitWithoutToken(TaskFunc&& taskfn, ResultFunc&& resultfn){
        auto task = MakeTask(std::forward<TaskFunc>(taskfn), std::forward<ResultFunc>(resultfn));
        SubmitWithoutToken(task);
    }
    void Submit(Task* task);
    void SubmitWithoutToken(Task* task);

    void SubmitBulk(LambdaTaskIteratorWrapper wrapper, size_t count);

    template<typename It, typename Func, typename ResultFunc,
             typename = std::enable_if_t<detail::has_operator_minus_v<It>>,
             typename = std::enable_if_t<std::is_copy_constructible_v<It>>>
    void ForEachRange(It begin, It end, Func &&func, ResultFunc &&resultfn) {
        size_t count = end - begin;
        ForEachN<It, Func>(begin, std::forward<Func>(func), std::move(resultfn), count);
    }

    template<typename It, typename Func, typename ResultFunc,
             typename = std::enable_if_t<detail::has_operator_plus_prefix_v<It>>,
             typename = std::enable_if_t<std::is_copy_constructible_v<It>>>
    void ForEachN(It begin, Func &&func, ResultFunc &&resultfn, size_t count) {
        std::shared_ptr<TaskIterator> iter(new LambdaTaskIterator<It, Func, ResultFunc>{
                std::move(begin), std::move(func), std::move(resultfn), count});
        SubmitBulk(LambdaTaskIteratorWrapper{std::move(iter)}, count);
    }

    int GetThreadCount() const;

    void Stop();

private:
    std::unique_ptr<ThreadPoolInternal> internal_;
}; 
} // namspace tl::blacklist
