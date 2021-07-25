#ifndef _CONTROLLER_H_
#define _CONTROLLER_H_

#include "logger.h"
#include "view.h"
#include <json11.hpp>
#include <map>

namespace tl::blacklist {
class Controller {
private:
  std::map<int, View *> m_modelMap;

public:
  Controller();
  ~Controller();
  void process(int fd, Json::Value &value);
};
} // namespace tl::blacklist
#endif
