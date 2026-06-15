# TAKT OS — Root CMake Project
# Requires ESP-IDF >= 5.0 for ESP32 targets
cmake_minimum_required(VERSION 3.16)

set(TAKT_OS_VERSION "0.2.0" CACHE STRING "TAKT OS version")
project(takt-os VERSION ${TAKT_OS_VERSION} LANGUAGES C CXX ASM)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

# TAKT OS component paths
set(TAKT_KERNEL_DIR   ${CMAKE_CURRENT_SOURCE_DIR}/kernel)
set(TAKT_DRIVERS_DIR  ${CMAKE_CURRENT_SOURCE_DIR}/drivers)
set(TAKT_MIDDLEWARE_DIR ${CMAKE_CURRENT_SOURCE_DIR}/middleware)
set(TAKT_SERVICES_DIR ${CMAKE_CURRENT_SOURCE_DIR}/services)

option(TAKT_BUILD_BOOTLOADER "Build bootloader component" ON)
option(TAKT_BUILD_RECOVERY  "Build recovery component"  ON)
option(TAKT_BUILD_TESTS     "Build unit tests"          OFF)

# Kernel is always built as a static library
add_subdirectory(kernel)

if(TAKT_BUILD_BOOTLOADER)
    add_subdirectory(takt_boot)
endif()

if(TAKT_BUILD_RECOVERY)
    add_subdirectory(recovery)
endif()

add_subdirectory(drivers)
add_subdirectory(middleware)
add_subdirectory(services)
add_subdirectory(examples)

if(TAKT_BUILD_TESTS)
    enable_testing()
    add_subdirectory(tests)
endif()
