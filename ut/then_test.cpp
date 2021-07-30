#include "tl/component/future.hh"
#include <iostream>
#include <string>
void getVersion() {
    std::string tag = "vr212123";
    std::string date = "210730";
    std::cout <<"start : "<<date<< std::endl;
    std::cout <<"version : "<< tag << std::endl;
}
int main() {
  using namespace tl::blacklist;
  future<void()> t(std::move(getVersion));
  t.then([](){
    std::cout <<"return 100 "<< std::endl;
    return 100;
  }).then([](auto j){
        std::cout <<"get last return  : "<<j<< std::endl;
  }).then([](){
      std::cout << "xiao yang say : " << std::endl;
      return std::string("fengfeng");
  }).then([](auto && s){
        std::cout <<s<<",yyds"<< std::endl;
        return 20000;
  }).then([](auto i){
     std::cout <<i<<" mobvista"<< std::endl;
  }).then([]{
    std::cout <<"byebye"<< std::endl;
  }).then([]{}).run();
  return 0;
}
