#include "tl/component/logger.h"
namespace tl::blacklist {
void init_tl_logger() {
  tl::blacklist::SerLogger::Instance();
  tl::blacklist::CliLogger::Instance();
  tl::blacklist::SqlLogger::Instance();
  tl::blacklist::DebugLogger::Instance();
}
}  // namespace tl::blacklist
