#include "logger.h"
#include "server.h"
#include <arpa/inet.h>
#include <netinet/in.h>
#include <sys/socket.h>

namespace tl::blacklist {
void listenClientConnectEventCallBack(int fd, short event, void *arg) {
  Server *ser = static_cast<Server *>(arg);
  if (EV_READ | event) {
    struct sockaddr_in cli;
    socklen_t len;
    int cfd = accept(fd, (struct sockaddr *)&cli, &len);
    if (-1 == cfd) {
      LOG_FUNC_MSG("accept()", errnoMap[errno]);
      exit(0);
    }
    write(ser->m_pool->getSubThreadSocketPairFd(), (char *)&cfd, 4);
  }
}

Server::Server(const std::string &ip, unsigned short port, int threadCnt) {
  m_listenFd = socket(AF_INET, SOCK_STREAM, 0);
  if (-1 == m_listenFd) {
    LOG_FUNC_MSG("socket()", errnoMap[errno]);
    exit(0);
  }

  struct sockaddr_in ser;
  ser.sin_family = AF_INET;
  ser.sin_port = htons(port);
  ser.sin_addr.s_addr = inet_addr("127.0.0.1");

  int res = bind(m_listenFd, (struct sockaddr *)&ser, sizeof(ser));
  if (-1 == res) {
    LOG_FUNC_MSG("bind()", errnoMap[errno]);
    exit(0);
  }
  res = listen(m_listenFd, 5);
  if (-1 == res) {
    LOG_FUNC_MSG("listen()", errnoMap[errno]);
    exit(0);
  }
  m_base = event_base_new();
  m_pool = new ThreadPool(threadCnt);
  m_tcpClient = new TcpClient();

  std::string loadBalanceServerIP;
  std::cout << "input loadBalanceServer ip:";
  std::cin >> loadBalanceServerIP;
  unsigned short loadBalanceServerPort;
  std::cout << "input loadBalanceServer port:";
  std::cin >> loadBalanceServerPort;
  m_tcpClient->Connect(loadBalanceServerIP, loadBalanceServerPort);
#if 0
        Json::Value value;
        value["ip"] = ip;
        value["port"] = port;
        m_tcpClient->Send(value.toStyledString());

        struct event* listenClinetConnectEvent = event_new(m_base, m_listenFd, EV_READ|EV_PERSIST, listenClientConnectEventCallBack, this);
        if (!listenClinetConnectEvent) {
            LOG_FUNC_MSG("event_new()", errnoMap[errno]);
            exit(0);
        }
        event_add(listenClinetConnectEvent, nullptr);
        event_base_dispatch(m_base);
#endif
}

Server::~Server() {
  delete m_pool;
  delete m_tcpClient;
  event_base_free(m_base);
  close(m_listenFd);
}
} // namespace tl::blacklist
