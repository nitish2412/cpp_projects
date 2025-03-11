#include "../hpp/Logger.hpp"
#include "spdlog/async.h"
#include "spdlog/sinks/rotating_file_sink.h"

// Define static member
std::shared_ptr<spdlog::logger> Logger::loggerInstance = nullptr;

std::shared_ptr<spdlog::logger> Logger::getInstance() {
    if (!loggerInstance) {  // ✅ Use class member correctly
        // Initialize the spdlog thread pool (1024 queue size, 1 background thread)
        spdlog::init_thread_pool(1024, 1);

        // Create a rotating file sink (5MB per file, keep last 3 files)
        auto rotating_sink = std::make_shared<spdlog::sinks::rotating_file_sink_mt>("logs/app.log", 5 * 1024 * 1024, 3);

        // ✅ Correctly assign to the class member
        loggerInstance = std::make_shared<spdlog::async_logger>(
            "file_logger", rotating_sink, spdlog::thread_pool(), spdlog::async_overflow_policy::block
        );

        spdlog::register_logger(loggerInstance);

        loggerInstance->set_pattern("[%Y-%m-%d %H:%M:%S] [%l] [%s:%# %!] %v");
        spdlog::flush_every(std::chrono::seconds(1));
        SPDLOG_LOGGER_INFO(loggerInstance, "Application started");

        // Set default log level
        loggerInstance->set_level(spdlog::level::info);
    }
    return loggerInstance;
}

// Set log level dynamically
void Logger::setLogLevel(spdlog::level::level_enum level) {
    if (loggerInstance) {
        loggerInstance->set_level(level);
    }
}
