#include "core/Log.h"

#include <string>
#include <iostream>

#include <spdlog/common.h>
#include <spdlog/spdlog.h>
#include <spdlog/sinks/stdout_color_sinks.h>
#include <spdlog/sinks/rotating_file_sink.h>

static std::shared_ptr<spdlog::logger> s_logger;

void SMLogger::Initialize(const char* loggerName) {    
    // LOGGING DIRECTLY TO CONSOLE 
    auto console = std::make_shared<spdlog::sinks::stdout_color_sink_mt>();
    console->set_color_mode(spdlog::color_mode::always);
    // pattern: LoggerName -> [Level] [Category] Message
    console->set_pattern("%n-> %^%l %v%$");
    console->set_color(spdlog::level::trace,    LogColor::TraceStyle);
    console->set_color(spdlog::level::debug,    LogColor::DebugStyle);
    console->set_color(spdlog::level::info,     LogColor::InfoStyle);
    console->set_color(spdlog::level::warn,     LogColor::WarnStyle);
    console->set_color(spdlog::level::err,      LogColor::ErrorStyle);
    console->set_color(spdlog::level::critical, LogColor::CriticalStyle);

    // LOGGING DIRECTLY TO FILEs
    // logs/...
    auto file = std::make_shared<spdlog::sinks::rotating_file_sink_mt>("logs/application.log", 1024*1024*5, 3);
    // pattern: [Weekday] [MonthName] [DayOfMonth] | [Hours] [Minutex] [Seconds] [Miliseconds] - [Category] Message
    file->set_pattern("[%a:%B:%d %f|%H:%M:%S:%e]-[%l]   %v");

    // Specifying features for logger
    spdlog::flush_every(std::chrono::seconds(2));
    s_logger = std::make_shared<spdlog::logger>(loggerName, spdlog::sinks_init_list{console, file});
#ifdef SM_TRACE
    s_logger->set_level(spdlog::level::trace);
    Log(LogLevel::Info, "LOG_INITIALIZE", "logger was successfully initialized, log_level is:{}", "trace");
#elifdef SM_DEBUG 
    s_logger->set_level(spdlog::level::debug);
    Log(LogLevel::Info, "LOG_INITIALIZE", "logger was successfully initialized, log_level is:{}", "debug");
#else
    s_logger->set_level(spdlog::level::info);
    Log(LogLevel::Info, "LOG_INITIALIZE", "logger was successfully initialized, log_level is:{}", "info");
#endif

}

void SMLogger::Write(LogLevel lvl, std::string_view category, std::string msg) {
    if (!s_logger) {
        std::cerr << " Logger was not initalized ";
    }
    auto full = std::string("[") + std::string(category) + "] " + msg;
    switch (lvl) {
    case LogLevel::Trace:    s_logger->trace(full); break;
    case LogLevel::Debug:    s_logger->debug(full); break;
    case LogLevel::Info:     s_logger->info(full); break;
    case LogLevel::Warn:     s_logger->warn(full); break;
    case LogLevel::Error:    s_logger->error(full); break;
    case LogLevel::Critical: s_logger->critical(full); break;
    }
}
