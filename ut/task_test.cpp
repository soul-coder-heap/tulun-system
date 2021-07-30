#include <iostream>
#include <type_traits>
#include "tl/component/task.hh"
#include "tl/component/apply.hh"

int main(){
    using namespace tl::blacklist;
    apply([](){std::cout <<"haha"<< std::endl;});
    auto t = apply([](){return 1;});
    apply([](int i, int j, double d){
        std::cout <<"i = " <<i<< ", j = "<<j<<", d = "<<d<<std::endl;
    },std::make_tuple(123,345564,3453.564));
    std::cout <<"t = "<<t<< std::endl;
    auto task = make_task([t]{
        std::cout <<"test"<< std::endl;
        return apply([&t]{return t;});
    },[](auto &&val){
        std::cout <<"test finish : "<<val<< std::endl;
    });
    (*task)();
    return 0;
}
