/*************************************************************************
	> File Name: ut/protobuf_test.cpp
	> Author: yafeng.zhao 
	> Mail: 1841531744@qq.com
	> Created Time: 日  8/ 8 18:07:25 2021
 ************************************************************************/

#include <iostream>
#include <string>
#include "message.pb.h"

int main()
{
    pt::rsp_login rsp{};
    rsp.set_ret(pt::rsp_login_RET_SUCCESS);
    auto user_info = rsp.mutable_user_info();
    user_info->set_nickname("dsw");
    user_info->set_icon("345DS55GF34D774S");
    user_info->set_coin(2000);
    user_info->set_location("zh");

    for (int i = 0; i < 5; i++) {
        auto record = rsp.add_record();
        record->set_time("2017/4/13 12:22:11");
        record->set_kill(i * 4);
        record->set_dead(i * 2);
        record->set_assist(i * 5);
    }

    std::string buff{};
    rsp.SerializeToString(&buff);
    //------------------解析----------------------
    pt::rsp_login rsp2{};
    if (!rsp2.ParseFromString(buff)) {
        std::cout << "parse error\n";
    }
    
    auto temp_user_info = rsp2.user_info();
    std::cout << "nickname:" << temp_user_info.nickname() << std::endl;
    std::cout << "coin:" << temp_user_info.coin() << std::endl;
    for (int m = 0; m < rsp2.record_size(); m++) {
        auto temp_record = rsp2.record(m);
        std::cout << "time:" << temp_record.time() << " kill:" << temp_record.kill() << " dead:" << temp_record.dead() << " assist:" << temp_record.assist() << std::endl;
    }
}
