#include <iostream>
#include "tl/component/threadpool_singleton.hh"
int main(){
    int n = 6;
    tl::blacklist::ThreadPoolSingleton::Init(n);
    using namespace tl::blacklist;
    int ii = 0;
    auto i = ThreadPoolSingleton::AsyncSubmit([&ii](){
        std::cout <<"submit "<< std::endl;
        ii++;
        return 100000;
    });
    /*.then([&ii](int i){
        std::cout <<"then submit : "<<i<< std::endl;
        ii++;
    }).then([&ii](){
        std::cout <<"2then submit"<< std::endl;
        ii++;
        return 100;
    }).then([&ii](auto i){
        std::cout <<"end : "<<i<< std::endl;
        ii++;
    }).run();
    std::cout <<"ii = "<<ii<< std::endl;
    std::cout <<"ii = "<<ii<< std::endl;
    std::cout <<"ii = "<<ii<< std::endl;
    std::cout <<"ii = "<<ii<< std::endl;
    std::cout <<"ii = "<<ii<< std::endl;
    std::cout <<"ii = "<<ii<< std::endl;
    */
    /*int64_t sum = 0;
    for(int i = 0; i < 1000000; i++){
        sum += i;
        std::cout <<ii<< std::endl;
    }*/
    //while(1){if(ii > 2){break;}}

    std::cout <<"i = "<<i.get()<< std::endl;
    return 0;

}

