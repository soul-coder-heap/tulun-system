#include "tl/component/future.hh"
#include <iostream>
int main() {
  using namespace tl::blacklist;
  future<int(int)> t([](int ii){std::cout <<"start"<< std::endl;return ii;});
  t.then([](auto i){
    std::cout <<"get i = "<<i<< std::endl;
    return 100;
  }).then([](auto j){
        std::cout <<"finishi : "<<j<< std::endl;
        //return 0;
  }).then([](){
      std::cout << "hahahhah" << std::endl;
      return 100;
  }).then([](auto i){
        std::cout <<"yyds : "<<i<< std::endl;
        return i;
  })//run(3);
  return 0;
}
