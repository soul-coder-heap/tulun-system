#include "transfer.h"
#include "logger.h"
#include <json11.hpp>
#include <unistd.h>
namespace tl::blacklist {
bool Transfer::recvFromMainSer(std::string &str) {
  return m_transferClient->Recv(str) <= 0 ? false : true;
}

bool Transfer::sendToMainSer(const std::string &msg) {
  return m_transferClient->Send(msg);
}

Transfer::Transfer() {
  LOG_FUNC_TRACE();
  m_transferClient = new TcpConnect();
  std::string loadBalanceServerIp = "127.0.0.1";
  unsigned short loadBalanceServerPort = 6300;
  m_transferClient->Connect(loadBalanceServerIp, loadBalanceServerPort);

  std::string msg;
  m_transferClient->Recv(msg);
#if 0
        Json::Value val;
        Json::Reader read;
        if (-1 == read.parse(msg.c_str(), val)) {
            LOG_FUNC_ERROR("json parse error");
            return;
        }
        std::string ip = val["ip"].asString();
        unsigned short port = val["port"].asInt();

        close(m_transferClient->getConnectFd());
        m_transferClient->Connect(ip, port);
#endif
}
} // namespace tl::blacklist
