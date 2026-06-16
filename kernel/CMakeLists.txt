# Copyright 2024-2026 Masyukov Pavel <p.masyukov@gmail.com>
# SPDX-License-Identifier: Apache-2.0
# TAKT OS — https://github.com/Developer-RU/Takt-OS

if(DEFINED ESP_PLATFORM OR DEFINED IDF_VER)
    idf_component_register(
        SRCS
            "src/event_bus.cpp"
            "src/timer.cpp"
            "src/scheduler.cpp"
            "src/kernel.cpp"
            "src/logger.cpp"
            "src/diagnostics.cpp"
            "src/storage_manager.cpp"
            "src/cache_manager.cpp"
            "src/firmware_cache.cpp"
            "src/nvs_manager.cpp"
            "src/crc32.cpp"
        INCLUDE_DIRS "include"
        REQUIRES nvs_flash esp_timer app_update esp_partition esp_system
    )
    target_compile_features(${COMPONENT_LIB} PUBLIC cxx_std_17)
    target_compile_definitions(${COMPONENT_LIB} PUBLIC TAKT_ESP32)
else()
    add_library(takt_kernel STATIC
        src/event_bus.cpp
        src/timer.cpp
        src/scheduler.cpp
        src/kernel.cpp
        src/logger.cpp
        src/diagnostics.cpp
        src/storage_manager.cpp
        src/cache_manager.cpp
    src/firmware_cache.cpp
    src/nvs_manager.cpp
    src/crc32.cpp
)
    target_include_directories(takt_kernel
        PUBLIC  ${CMAKE_CURRENT_SOURCE_DIR}/include
        PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/src
    )
    target_compile_features(takt_kernel PUBLIC cxx_std_17)
    target_compile_definitions(takt_kernel PUBLIC TAKT_HOST_BUILD)
endif()
