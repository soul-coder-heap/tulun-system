#ifndef _PUBLIC_H_
#define _PUBLIC_H_
#include <iostream>

namespace tl::blacklist {
#define LOG(str)                                                               \
  std::cout << __FILE__ << ":" << __LINE__ << __TIMESTAMP__ << " " << str      \
            << "\n";

} // namespace tl::blacklist
#endif
