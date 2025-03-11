#ifndef LOGGER_H
#define LOGGER_H

#include "spdlog/spdlog.h"
#include "spdlog/sinks/basic_file_sink.h"
#include <memory>

class Logger {
public:
    // Get the singleton logger instance
    static std::shared_ptr<spdlog::logger> getInstance();
    static void setLogLevel(spdlog::level::level_enum level);

private:
    Logger() = default;  // Private constructor to prevent multiple instances
    static std::shared_ptr<spdlog::logger> loggerInstance;
};

#endif // LOGGER_H

