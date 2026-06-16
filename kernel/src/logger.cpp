// Copyright 2024-2026 Masyukov Pavel <p.masyukov@gmail.com>
// SPDX-License-Identifier: Apache-2.0
//
// TAKT OS — https://github.com/Developer-RU/Takt-OS

#include "takt/logger.hpp"
#include <cstdio>
#include <cstdarg>

#ifdef TAKT_ESP32
#include "esp_log.h"
#endif

namespace takt {

void Logger::log(LogLevel level, const char* tag, const char* fmt, ...) const {
    if (static_cast<uint8_t>(level) < static_cast<uint8_t>(level_)) return;

    va_list args;
    va_start(args, fmt);

#ifdef TAKT_ESP32
    esp_log_level_t espLevel;
    switch (level) {
        case LogLevel::Trace:
        case LogLevel::Debug: espLevel = ESP_LOG_DEBUG; break;
        case LogLevel::Info:  espLevel = ESP_LOG_INFO;  break;
        case LogLevel::Warn:  espLevel = ESP_LOG_WARN;  break;
        case LogLevel::Error:
        case LogLevel::Fatal: espLevel = ESP_LOG_ERROR; break;
        default: espLevel = ESP_LOG_NONE; break;
    }
    esp_log_writev(espLevel, tag, fmt, args);
#else
    static const char* levelStr[] = {"T", "D", "I", "W", "E", "F"};
    uint8_t idx = static_cast<uint8_t>(level);
    if (idx > 5) idx = 5;
    std::printf("[%s][%s] ", levelStr[idx], tag);
    std::vprintf(fmt, args);
    std::printf("\n");
#endif

    va_end(args);
}

} // namespace takt
