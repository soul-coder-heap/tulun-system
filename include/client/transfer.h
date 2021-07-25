#ifndef __WORKSER_H__
#define __WORKSER_H__

#include "tcpConnect.h"
//单例模式
namespace tl::blacklist {
class Transfer {
public:
  bool recvFromMainSer(std::string &msg);
  bool sendToMainSer(const std::string &msg);

  static Transfer *getInstance() {
    static Transfer transfer;
    return &transfer;
  }

private:
  Transfer();
  TcpConnect *m_transferClient;
};

} // namespace tl::blacklist
#endif
