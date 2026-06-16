# Copyright 2024-2026 Masyukov Pavel <p.masyukov@gmail.com>
# SPDX-License-Identifier: Apache-2.0
# TAKT OS — https://github.com/Developer-RU/Takt-OS

# TAKT OS Lite — reduced footprint for resource-constrained targets.
# Targets: ESP8266-class (future), ESP32 with minimal config.

cmake_minimum_required(VERSION 3.16)

if(DEFINED ESP_PLATFORM OR DEFINED IDF_VER)
    idf_component_register(
        SRCS
            "../kernel/src/scheduler.cpp"
            "../kernel/src/event_bus.cpp"
            "../kernel/src/timer.cpp"
            "../kernel/src/logger.cpp"
            "../kernel/src/kernel_lite.cpp"
        INCLUDE_DIRS "../kernel/include"
        REQUIRES esp_timer
    )
    target_compile_definitions(${COMPONENT_LIB} PUBLIC TAKT_LITE TAKT_ESP32)
    target_compile_features(${COMPONENT_LIB} PUBLIC cxx_std_17)
endif()
