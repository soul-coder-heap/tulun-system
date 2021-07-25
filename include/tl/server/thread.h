#ifndef _THREAD_H_
#define _THREAD_H_

#include "controller.h"
#include "logger.h"
#include <event.h>
#include <iostream>
#include <json11.hpp>
#include <map>
#include <pthread.h>
#include <sys/types.h>
#include <unistd.h>

namespace tl::blacklist {
class Thread {
private:
  pthread_t m_id;
  std::map<int, struct event *> *m_eventMap;
  int m_socketpairFd[2];
  struct event_base *m_base;
  Controller *m_controller;
  friend void clientIOEventCallBack(int fd, short event, void *arg);
  friend void socketPairIOEventCallBack(int fd, short event, void *arg);
  friend void *threadTaskFunction(void *arg);

public:
  Thread();
  ~Thread();
  int getEventMapSize();
  int getSocketPairFdFirst();
  int getSocketPairFdSecond();
  pthread_t getId();
};
} // namespace tl::blacklist
#endif
