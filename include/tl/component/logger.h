#pragma once
#include <spdlog/async.h>
#include <spdlog/sinks/rotating_file_sink.h>
#include <spdlog/spdlog.h>
#include <chrono>
#include <cstring>
#include <iostream>
#include "tl/component/singleton.hh"

namespace tl::blacklist {

#ifndef LOG_INFO
#define LOG_INFO(format, args...)                                                 \
  SPDLOG_INFO(format, ##args);
#endif

#ifndef LOG_ERROR
#define LOG_ERROR(format, args...)                                                \
  SPDLOG_ERROR(format, ##args);
#endif


#define TL_LOGGER(class_name, path, max_cut_size, format, level)         \
  class class_name : public tl::blacklist::Singleton<class_name, true> { \
   public:                                                               \
    class_name() {                                                       \
      file_logger = spdlog::rotating_logger_mt<spdlog::async_factory>(   \
          #class_name, path, max_cut_size, 5);                           \
      file_logger->set_pattern(format);                                  \
      file_logger->set_level(level);                                     \
    }                                                                    \
    spdlog::logger *GetLogger() { return file_logger.get(); }            \
                                                                         \
   private:                                                              \
    std::shared_ptr<spdlog::logger> file_logger;                         \
  };

TL_LOGGER(SerLogger, "log/service.log", 30 * 1024 * 1024, "%v",
          spdlog::level::info);
TL_LOGGER(SqlLogger, "log/sql_operator.log", 30 * 1024 * 1024,
          "[%m%d-%H:%M:%S:%e %z][%n][%l][T%t][%@] %v", spdlog::level::info);
TL_LOGGER(CliLogger, "log/client.log", 30 * 1024 * 1024, "%v",
          spdlog::level::info);
TL_LOGGER(DebugLogger, "log/debug.log", 128 * 1024 * 1024,
          "[%m%d-%H:%M:%S:%e %z][%n][%l][T%t][%@] %v", spdlog::level::info);
// clang-format off
// server information
#define LOG_SER(format, args...) \
  SPDLOG_LOGGER_INFO(SerLogger::Instance()->GetLogger(), format, ##args);

// client information
#define LOG_CLI(format, args...) \
  SPDLOG_LOGGER_INFO(CliLogger::Instance()->GetLogger(), format, ##args);
// sql curd
#define LOG_SQL(format, args...) \
  SPDLOG_LOGGER_INFO(SqlLogger::Instance()->GetLogger(), format, ##args);

// please make compile.sh NO_DEBUG=0
#ifndef NO_DEBUG
#define LOG_DEBUG(format, args...) \
  SPDLOG_LOGGER_INFO(DebugLogger::Instance()->GetLogger(), format, ##args);
#endif

void init_tl_logger();
}  // namespace tl::blacklist
// clang-format on
