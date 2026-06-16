# Copyright 2024-2026 Masyukov Pavel <p.masyukov@gmail.com>
# SPDX-License-Identifier: Apache-2.0
# TAKT OS — https://github.com/Developer-RU/Takt-OS

if(DEFINED ESP_PLATFORM OR DEFINED IDF_VER)
    idf_component_register(
        SRCS
            "src/platform.cpp"
            "src/uart.cpp"
            "src/adc.cpp"
            "src/flash_backend.cpp"
        INCLUDE_DIRS "include"
        REQUIRES driver esp_system esp_adc esp_partition kernel app_update
    )
    target_compile_definitions(${COMPONENT_LIB} PUBLIC TAKT_ESP32)
else()
    add_library(takt_drivers STATIC
        src/platform.cpp
        src/uart.cpp
        src/adc.cpp
        src/flash_backend.cpp
    )
    target_include_directories(takt_drivers PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/include)
    target_link_libraries(takt_drivers PUBLIC takt_kernel)
endif()
