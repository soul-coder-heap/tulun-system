#ifndef __TCPSERVER_H__
#define __TCPSERVER_H__
#include <string>

namespace tl::blacklist {
class TcpServer {
public:
  TcpServer(const std::string &ip, unsigned short port);
  int Accept();

  int Send(int fd, const std::string &msg);
  int Recv(int fd, std::string &msg);
  int getListenFd() const;
  unsigned short getPort() const;
  std::string getIp() const;

private:
  int m_listenFd;
  unsigned short m_port;
  std::string m_ip;
};

} // namespace tl::blacklist
#endif
