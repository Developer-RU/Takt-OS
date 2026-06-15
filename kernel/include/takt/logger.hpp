#pragma once

#include <cstddef>
#include <cstdint>

namespace takt {

/// Log severity levels.
enum class LogLevel : uint8_t {
    Trace = 0,
    Debug = 1,
    Info  = 2,
    Warn  = 3,
    Error = 4,
    Fatal = 5,
    None  = 6,
};

/// Compile-time log level filter.
#ifndef TAKT_LOG_LEVEL
#define TAKT_LOG_LEVEL takt::LogLevel::Debug
#endif

class Logger {
public:
    static Logger& instance() {
        static Logger log;
        return log;
    }

    void setLevel(LogLevel level) { level_ = level; }
    LogLevel level() const { return level_; }

    void log(LogLevel level, const char* tag, const char* fmt, ...) const;

private:
    LogLevel level_ = LogLevel::Debug;
};

} // namespace takt

#define TAKT_LOG(level, tag, ...) \
    do { \
        if (static_cast<uint8_t>(level) >= static_cast<uint8_t>(TAKT_LOG_LEVEL)) { \
            takt::Logger::instance().log(level, tag, __VA_ARGS__); \
        } \
    } while (0)

#define TAKT_LOGI(tag, ...) TAKT_LOG(takt::LogLevel::Info,  tag, __VA_ARGS__)
#define TAKT_LOGW(tag, ...) TAKT_LOG(takt::LogLevel::Warn,  tag, __VA_ARGS__)
#define TAKT_LOGE(tag, ...) TAKT_LOG(takt::LogLevel::Error, tag, __VA_ARGS__)
#define TAKT_LOGD(tag, ...) TAKT_LOG(takt::LogLevel::Debug, tag, __VA_ARGS__)
