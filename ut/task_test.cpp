#include <iostream>
#include <type_traits>
#include "tl/component/task.hh"

template <typename Func>
static inline auto apply(Func && func){
    if(std::is_same_v<void, std::invoke_result_t<Func>>){
        func();
    }else{
        return func();
    }
}

int main(){
    using namespace tl::blacklist;
    apply([](){std::cout <<"haha"<< std::endl;});
    auto t = apply([](){return 1;});
    std::cout <<"t = "<<t<< std::endl;
    auto task = make_task([t]{
        std::cout <<"test"<< std::endl;
        return t * 10001;
    },[](auto &&val){
        std::cout <<"test finish : "<<val<< std::endl;
    });
    (*task)();
}
