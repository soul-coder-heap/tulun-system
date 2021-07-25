#include "tcpServer.h"
#include <arpa/inet.h>
#include <event.h>
#include <iostream>
#include <json11.hpp>
#include <map>
#include <netinet/in.h>
#include <openssl/md5.h>
#include <pthread.h>
#include <string.h>
#include <string>
#include <sys/socket.h>

namespace tl::blacklist {
class PhysicalNode {
private:
  int m_cnt;
  std::string m_ip;
  unsigned short m_port;

public:
  PhysicalNode();
  PhysicalNode(int cnt, const std::string &io, unsigned short port);
  int getVirtualNodeCnt();
  void setPhysicalNode(const std::string &ip, unsigned short port);
  string getIp();
  unsigned short getPort();
};

class VirtualNode {
private:
  long m_hashValue; // 虚拟节点对应的hash值
  PhysicalNode *m_father;

public:
  VirtualNode();
  VirtualNode(PhysicalNode *father);
  void setVirtualNode(PhysicalNode *father);
  PhysicalNode *getFatherPhysicalNode();
  void setHash(long hash);
  long getHash();
};

class HashFunction {
public:
  virtual long getHashValue(const std::string &sock) = 0;
};

class MD5HashFunction : public HashFunction {
  virtual long getHashValue(const std::string &sock);
};

class ConsistentHashCircle {
public:
  ConsistentHashCircle(HashFunction *fun);
  ~ConsistentHashCircle();
  void setHashFunction(HashFunction *fun);
  int addVirtualNode(PhysicalNode *node);
  int removeVirtualNode(PhysicalNode *node);
  int getVirtualNodeCnt();
  PhysicalNode *searchPhysicalNode(const std::string &sock);

private:
  HashFunction *m_fun;
  int m_virtualNodeCnt;
  std::map<unsigned long, VirtualNode *> *m_virtualNodeMap;
};

class LoadBalanceServer {
public:
  LoadBalanceServer();
  static void ioEventCallBack(int fd, short event, void *arg);
  static void listenClientCallBack(int fd, short event, void *arg);
  static void listenServerCallBack(int fd, short event, void *arg);

private:
  static TcpServer *m_server;
  static TcpServer *m_client;
  static event_base *m_base;
  static ConsistentHashCircle *m_consistentHashCircle;
  static std::map<int, PhysicalNode *> *m_physicalNodeMap;
  static std::map<int, struct event *> *m_eventMap;
};
} // namespace tl::blacklist
