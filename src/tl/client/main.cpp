
#include "informationCollectorServerThread.h"
#include "logger.h"
#include "system.h"
#include "transfer.h"
#include <iostream>

namespace tl::blacklist {
typedef void (System::*PFUNC)();
typedef struct {
  int type;
  PFUNC pfunc;
} SystemHandler;

SystemHandler systemHandlerMap[] = {{EN_REGISTER, &System::Register},
                                    {EN_LOGIN, &System::Login}};

int main() {
  System *s = new System();
  sleep(1);

  for (;;) {
    s->menu();
    std::cout << "please input your type:";
    int type;
    std::cin >> type;
    getchar();
    int size = sizeof(systemHandlerMap) / sizeof(SystemHandler);
    int i;
    for (i = 0; i < size; ++i) {
      if (type == systemHandlerMap[i].type) {
        (s->*systemHandlerMap[i].pfunc)();
      }
    }
    if (i > size)
      continue;
  }

  delete s;
  return 0;
}
} // namespace tl::blacklist
