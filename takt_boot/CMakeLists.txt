# Copyright 2024-2026 Masyukov Pavel <p.masyukov@gmail.com>
# SPDX-License-Identifier: Apache-2.0
# TAKT OS — https://github.com/Developer-RU/Takt-OS

if(DEFINED ESP_PLATFORM OR DEFINED IDF_VER)
    idf_component_register(
        SRCS "src/bootloader.cpp"
        INCLUDE_DIRS "include"
        REQUIRES kernel driver app_update esp_partition nvs_flash
    )
    target_compile_definitions(${COMPONENT_LIB} PUBLIC TAKT_ESP32)
else()
    add_library(takt_bootloader STATIC src/bootloader.cpp)
    target_include_directories(takt_bootloader PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/include)
    target_link_libraries(takt_bootloader PUBLIC takt_kernel)
endif()
