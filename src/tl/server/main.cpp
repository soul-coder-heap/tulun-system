#include <iostream>
#include <string>
#include <algorithm>
#include <future>
#include <thread>
#include "tl/component/logger.h"
#include "tl/component/future.hh"
int main(){
    using namespace tl::blacklist;
    LOG_INFO("hello tulun blacklist syetem");
    tl::blacklist::init_tl_logger();
    LOG_SER("func = {}\tversion = {}","main","master");
    LOG_SQL("query success");
    auto f1 = std::async([](){
      for(int i = 0; i < 100; i++){
        if(i % 3 == 0){
          LOG_SQL("no.{} query success", i);
        }else{
          LOG_SQL("no.{} insert success", i);
        }
      }
      return 400;
    });
  auto t = f1.get();
  std::thread t1([t](){
    for(int i = 0; i< 100; i++)
      LOG_SER("no.{} sql operator success, result {}",i,t);
  });
  t1.join();
  /*future<int()> f([](){
        LOG_INFO("start service ...");
        return 0b0111;
    });
  f.then([](auto val){
         return 0b010101 + val;
  }).then([](auto val){
          LOG_INFO("获取 value : {}", val);
          return 0;
  }).get();
  */
  return 0;
}

