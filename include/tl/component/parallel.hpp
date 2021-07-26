#ifndef _PARALLEL_HPP
#define _PARALLEL_HPP
#include <functional>
#include <future>
#include <thread>
namespace tl::blacklist{
template <class T> 
class Task;

template<class R, class...Args>
class Task<R(Args...)>
{
private:
    std::function<R(Args...)> m_fn;
public:
    typedef R return_type;
    Task(std::function<R(Args...)>&& f) :m_fn(std::move(f)) {}
    Task(std::function<const R(Args...)>& f) :m_fn(f) {}
    ~Task(){}
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
    auto then(F&& f)->Task<typename std::result_of<F(R)>::type(Args...)>
    {
        typedef typename std::result_of<F(R)>::type  ReturnType;
        auto func = std::move(m_fn);
        return Task<ReturnType(Args...)>([func, &f](Args&& ...args){
            std::future<R> lastf = std::async(func, std::forward<Args>(args)...);
            return std::async(f, lastf.get()).get();
        });
    }
};
}
#endif
