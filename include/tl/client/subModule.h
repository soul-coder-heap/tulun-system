#ifndef __SUB_MODULE_H__
#define __SUB_MODULE_H__
#include "informationCollectorServerThread.h"
#include "logger.h"
#include "transfer.h"
#include <iostream>
#include <json11.hpp>
#include <pthread.h>

namespace tl::blacklist {
class SubModule {
public:
  SubModule();
  ~SubModule();
  void menu();
  void startSuModuleThread();
  void Delete();
  void Insert();
  void Update();
  void Search();
  void Exit();

  static void *recvFromSubModule(void *arg) {
    LOG_FUNC_TRACE();
    InformationCollectorServerThread *informationCollectorThread =
        static_cast<InformationCollectorServerThread *>(arg);
#if 0
                for(;;) {
                    string str;
                    if (Transfer::getInstance()->recvFromMainSer(str)) {

                        LOG_SOURCE_MSG("Mainserver", str.c_str());
                        Json::Value val;
                        Json::Reader read;

                        if (!read.parse(str ,val)) {
                            LOG_FUNC_MSG("json reader parse error", errnoMap[errno]);
                            continue;
                        }

                        if (val["type"].asInt()==EN_INFORMATION_COLLECTOR && val["fd"]!=-1) {
                            cout<<"即将发送数据给信息采集器!"<<endl;
                            int fd = val["fd"].asInt();
                            string mes = val["message"].asString();
                            informationCollectorThread->sendToInformationCollector(fd, str);
                        }

                        else{
                            cout<<val["message"].asString()<<endl;
                        }
                    }
                }
#endif
  }

private:
  InformationCollectorServerThread
      *m_informationCollectorThread; // 信息采集机的服务器
};

} // namespace tl::blacklist
#endif
