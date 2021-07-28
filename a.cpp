/*************************************************************************
	> File Name: a.cpp
	> Author: yafeng.zhao 
	> Mail: 1841531744@qq.com
	> Created Time: 三  7/28 23:14:04 2021
 ************************************************************************/

#include <iostream>
#include <type_traits>

template <typename Func>
static inline auto apply(Func && func){
    if(std::is_same_v<void, std::invoke_result_t<Func>>){
        func();
    }else{
        return func();
    }
}

int main(){
    apply([](){std::cout <<"haha"<< std::endl;});
    auto t = apply([](){return 1;});
    std::cout <<"t = "<<t<< std::endl;
}
