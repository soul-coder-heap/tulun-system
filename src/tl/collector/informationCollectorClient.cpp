#include "logger.h"
#include <arpa/inet.h>
#include <assert.h>
#include <errno.h>
#include <fcntl.h>
#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <string>
#include <sys/socket.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>

namespace tl::blacklist {
int main() {
  int sockfd = socket(AF_INET, SOCK_STREAM, 0);
  if (-1 == sockfd) {
    LOG_FUNC_MSG("socket()", errnoMap[errno]);
    exit(0);
  }

  struct sockaddr_in ser;
  ser.sin_family = AF_INET;

  std::string ip = "";
  std::cout << "please input information collector server ip address:";
  std::cin >> ip;

  unsigned short port = -1;
  std::cout << "please input information collector server port:";
  std::cin >> port;
  ser.sin_port = htons(port);
  ser.sin_addr.s_addr = inet_addr(ip.c_str());

  if (-1 == connect(sockfd, (struct sockaddr *)&ser, sizeof(ser))) {
    LOG_FUNC_MSG("connect()", errnoMap[errno]);
    exit(0);
  }

  std::string id = "";
  std::cout << "please input the tourist's id:";
  std::cin >> id;
  send(sockfd, id.c_str(), strlen(id.c_str()), 0);

  char buf[128] = {0};

  recv(sockfd, buf, 127, 0);
  std::cout << buf << std::endl;

  return 0;
}
} // namespace tl::blacklist
