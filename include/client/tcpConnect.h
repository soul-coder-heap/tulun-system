#ifndef __TCPCONNECT_H__
#define __TCPCONNECT_H__

#include <arpa/inet.h>
#include <cassert>
#include <cstdio>
#include <cstring>
#include <errno.h>
#include <iostream>
#include <string>
#include <sys/socket.h>

namespace tl::blacklist {
class TcpConnect {
private:
  int m_connectFd;
  std::string m_ip;
  uint16_t m_port;

public:
  int getConnectFd() const;
  uint16_t getPort() const;
  std::string getIp() const;

  int Connect(const std::string &ip, unsigned short port);
  ~TcpConnect();
  int Send(const std::string &msg);
  int Recv(std::string &msg);
};
} // namespace tl::blacklist
#endif
