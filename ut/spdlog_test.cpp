#include <iostream>
#include <string>
#include <algorithm>
#include "tl/component/logger.h"
int main(){
    using namespace tl::blacklist;
    std::cout <<"hello tulun blacklist syetem"<< std::endl;
    tl::blacklist::init_tl_logger();
    LOG_SER("func = {}\tversion = {}","main","master");
    LOG_SQL("query success");
    return 0;
}
