#ifndef __SYSTEM_H__
#define __SYSTEM_H__

#include "logger.h"
#include "subModule.h"
#include <iostream>
#include <json11.hpp>
namespace tl::blacklist {
class System {
public:
  System();
  ~System();

  void menu();
  void Register();
  void Login();
  void Exit();

private:
  SubModule *m_subModule;
};

} // namespace tl::blacklist
#endif
