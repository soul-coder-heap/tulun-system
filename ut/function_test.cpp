#include <iostream>
#include <string>
#include <algorithm>
#include <type_traits>
#include <typeinfo>
#include <functional>
#include "tl/component/function_traits.hh"
#include "tl/component/logger.h"
class test{
    public:
        void info(){}
        void operator()(){}
};
int f() {return 0;}
int main(){
    using namespace tl::blacklist;
    auto f1 = [](int a,double b, std::string c){
        LOG_ERROR("a = {}, b = {}, c = {}", a,b,c);
        return a * b;
    };
    LOG_INFO("f1 result = {}", f1(10, 23.4, "mutiple"));
    
    using f1type = function_traits<decltype(f1)>::return_type;
    using tuptype1 = function_traits<decltype(f1)>::args_as_tuple;
    tuptype1 t1;
    LOG_INFO("tuptype1 type = {}, size = {}",typeid(t1).name(), std::tuple_size<tuptype1>{});

    if constexpr (std::is_same<f1type, decltype(f1)>::value){
        LOG_INFO("type same");
    }else{
        LOG_INFO("type different");
        LOG_INFO("f1type type name : {}", typeid(f1type{}).name());
        LOG_INFO("f1 type name : {}", typeid(f1).name());
        std::function<double(int,int,int)> f2;
        LOG_INFO("decltypef1 type name : {}", typeid(f2).name());
    }

    function_traits<decltype(f1)> df1;
    LOG_INFO("xixi");
    function_traits<void()> df2;
    LOG_INFO("xinxin");
    function_traits<decltype(&test::info)> df3;
    LOG_INFO("xinxin1");
    function_traits<decltype(f)> df4;
    LOG_INFO("xinxin2");
    function_traits<decltype(test())> df5;
    LOG_INFO("xinxin3");
    function_traits<decltype(std::function<void(int, int, double)>{})> df6;
    function_traits<decltype(f1)>::get_type<1> a{};
    function_traits<void(int, long, double)>::get_type<1> b{};
    //LOG_INFO("f1type get<1> : {}", a);
    //LOG_INFO("<void(int,int,double)> get<1> : {}, type is {}", b, typeid(b).name());
    return 0;    
}
