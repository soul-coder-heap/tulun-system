#ifndef _TCPCLIENT_H_
#define _TCPCLIENT_H_

#include <arpa/inet.h>
#include <netinet/in.h>
#include <string.h>
#include <string>
#include <sys/socket.h>
#include <unistd.h>

namespace tl::blacklist {
class TcpClient {
private:
  std::string m_ip;
  unsigned short m_port;
  int m_fd;

public:
  int getFd() const;
  unsigned short getPort() const;
  std::string getIp() const;

  TcpClient();
  ~TcpClient();
  int Connect(const std::string &ip, unsigned short port);
  int Send(const std::string &msg);
  int Recv(std::string &msg);
};
} // namespace tl::blacklist
#endif
