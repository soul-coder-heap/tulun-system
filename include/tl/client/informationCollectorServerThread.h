#ifndef __SELECTSERTHREAD_H__
#define __SELECTSERTHREAD_H__

#include "logger.h"
#include "tcpServer.h"
#include "transfer.h"
#include <event.h>
#include <iostream>
#include <json11.hpp>
#include <map>
#include <pthread.h>
#include <string>
#include <unistd.h>
namespace tl::blacklist {
class InformationCollectorServerThread {
public:
  /*
description:监听信息采集器连接的回调事件
创建处理信息采集器io事件

*/
  static void listenInformationCollectorConnectCallBack(int fd, short event,
                                                        void *arg) {
    LOG_FUNC_TRACE();
    TcpServer *server = static_cast<TcpServer *>(arg);
    int cfd = server->Accept();
    if (-1 == cfd) {
      LOG_FUNC_MSG("server->accept()", errnoMap[errno]);
      return;
    }

    struct event *ev = event_new(
        m_base, cfd, EV_READ | EV_PERSIST,
        InformationCollectorServerThread::informationCollectorIOEventCallBack,
        server);
    if (nullptr == ev) {
      LOG_FUNC_MSG("event_new()", errnoMap[errno]);
      return;
    }
    m_eventMap.insert(make_pair(cfd, ev));
    event_add(ev, nullptr);
  }

  static void informationCollectorIOEventCallBack(int fd, short event,
                                                  void *arg) {
    LOG_FUNC_TRACE();
    TcpServer *server = static_cast<TcpServer *>(arg);
    string id;
    if (server->Recv(fd, id) <= 0) {
      LOG("informationCollector client shutdown!");
      event_free(m_eventMap[fd]);
      m_eventMap.erase(fd);
      close(fd);
      return;
    }
#if 0
                json11::Json::Value val;
                val["type"] = EN_INFORMATION_COLLECTOR;
                val["fd"] = fd;
                val["message"] = id.c_str();

                cout<<"selecter data:"<<val;
                cout<<"将信息采器的数据发送给主服务器!"<<endl;
                Transfer::getInstance()->sendToMainSer(val.toStyledString());
                cout<<"信息采集机器的数据发送给主服务器完毕!"<<endl;
#endif
  }

  static void *startInformationCollectorServerThread(void *arg) {
    LOG_FUNC_TRACE();
    struct event_base *base = static_cast<struct event_base *>(arg);
    event_base_dispatch(base);
  }

  InformationCollectorServerThread();
  void sendToInformationCollector(int fd, std::string &msg);

private:
  TcpServer *m_informationCollectorServer;
  static struct event_base *m_base;
  static std::map<int, struct event *> m_eventMap;
};

} // namespace tl::blacklist
#endif
