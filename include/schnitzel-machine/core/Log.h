#ifndef LOG_H_
#define LOG_H_

#include <string_view>
#include <format>

namespace LogColor {
    inline constexpr const char* TraceStyle    = "\033[2;37m";      // dim white 
    inline constexpr const char* DebugStyle    = "\033[36m";        // cyan
    inline constexpr const char* InfoStyle     = "\033[32m";        // green
    inline constexpr const char* WarnStyle     = "\033[1;33m";      // bold yellow 
    inline constexpr const char* ErrorStyle    = "\033[1;31m";      // bold red 
    inline constexpr const char* CriticalStyle = "\033[1;37;41m";   // bold white on red bg
}
enum class LogLevel {
    Trace, Debug, Info, Warn, Error, Critical
};

class SMLogger {
public:
    SMLogger(const SMLogger&) = delete;
    SMLogger& operator=(const SMLogger&) = delete;

    static SMLogger& Get() {
        static SMLogger instance;  
        return instance;
    }

    void Initialize(const char* loggerName);
    void Write(LogLevel lvl, std::string_view category, std::string msg);

    template<typename... Args>
    void Log(LogLevel lvl, std::string_view category, std::format_string<Args...> fmt, Args&&... args) {
        Write(lvl, category, std::format(fmt, std::forward<Args>(args)...));
    }

private:
    SMLogger() = default;
};

#define SM_LOG_INIT(name)               SMLogger::Get().Initialize(name)

#define SM_LOG_TRACE(cat, ...)          SMLogger::Get().Log(LogLevel::Trace,    cat, __VA_ARGS__)
#define SM_LOG_DEBUG(cat, ...)          SMLogger::Get().Log(LogLevel::Debug,    cat, __VA_ARGS__)
#define SM_LOG_INFO(cat, ...)           SMLogger::Get().Log(LogLevel::Info,     cat, __VA_ARGS__)
#define SM_LOG_WARN(cat, ...)           SMLogger::Get().Log(LogLevel::Warn,     cat, __VA_ARGS__)
#define SM_LOG_ERROR(cat, ...)          SMLogger::Get().Log(LogLevel::Error,    cat, __VA_ARGS__)
#define SM_LOG_CRITICAL(cat, ...)       SMLogger::Get().Log(LogLevel::Critical, cat, __VA_ARGS__)

#endif // LOG_H_
