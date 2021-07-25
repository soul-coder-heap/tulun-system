
#include "informationCollectorServerThread.h"
namespace tl::blacklist {
std::map<int, struct event *> InformationCollectorServerThread::m_eventMap =
    std::map<int, struct event *>();
struct event_base *InformationCollectorServerThread::m_base = nullptr;

//信息采集器线程函数用来处理信息采集机器的连接
InformationCollectorServerThread::InformationCollectorServerThread() {
  LOG_FUNC_TRACE();
  std::string ip = "127.0.0.1";
  unsigned short port = 6000;
  m_informationCollectorServer = new TcpServer(ip, port);
  m_base = event_base_new();

  //监听信息采集器连接的事件处理器
  struct event *ev = event_new(
      m_base, m_informationCollectorServer->getListenFd(), EV_READ | EV_PERSIST,
      InformationCollectorServerThread::
          listenInformationCollectorConnectCallBack,
      m_informationCollectorServer);

  if (ev == nullptr) {
    LOG_FUNC_MSG("event_base()", errnoMap[errno]);
    return;
  }
  event_add(ev, nullptr);

  pthread_t tid;
  pthread_create(
      &tid, nullptr,
      InformationCollectorServerThread::startInformationCollectorServerThread,
      (void *)m_base);
}

void InformationCollectorServerThread::sendToInformationCollector(
    int fd, std::string &msg) {
  LOG_FUNC_TRACE();
  m_informationCollectorServer->Send(fd, msg);
}

} // namespace tl::blacklist
