#pragma once

#include <memory>
#include <type_traits>

namespace tl::blacklist {
class task {
public:
    explicit task() {}
    virtual ~task() noexcept {}
    virtual void operator()() noexcept = 0;
};


template <typename Func, typename Result>
class lambda_task final : public task {
    Func _func;
    Result _result;
public:
    lambda_task(Func&& func, Result&& result) : _func(std::move(func)), _result(std::move(result)) {}
    virtual void operator()() noexcept override {
		if constexpr(std::is_same_v<void, std::invoke_result_t<Func>>){
        	_func();
            _result();
		}else{
			auto r = _func();
            _result(r);
		}
    }
};

template <typename Func, typename Result>
inline std::unique_ptr<task>
make_task(Func&& func, Result&& result) {
    return std::make_unique<lambda_task<Func, Result>>(std::forward<Func>(func), std::forward<Result>(result));
}
}// namespace tl::blacklist
