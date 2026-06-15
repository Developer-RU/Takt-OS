# TAKT OS Unit Tests
cmake_minimum_required(VERSION 3.16)
project(takt_tests LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)

add_subdirectory(.. takt_os EXCLUDE_FROM_ALL)

enable_testing()
find_package(GTest QUIET)

if(NOT GTest_FOUND)
    include(FetchContent)
    FetchContent_Declare(googletest URL https://github.com/google/googletest/archive/refs/tags/v1.14.0.zip)
    FetchContent_MakeAvailable(googletest)
endif()

add_executable(takt_tests
    test_scheduler.cpp
    test_event_bus.cpp
    test_timer.cpp
    test_nvs.cpp
    test_firmware_cache.cpp
    test_bootloader.cpp
)

target_link_libraries(takt_tests PRIVATE takt_kernel)
target_include_directories(takt_tests PRIVATE
    ${CMAKE_SOURCE_DIR}/../takt_boot/include
)
target_sources(takt_tests PRIVATE
    ${CMAKE_SOURCE_DIR}/../takt_boot/src/bootloader.cpp
)
target_compile_definitions(takt_tests PRIVATE TAKT_HOST_BUILD)

include(GoogleTest)
gtest_discover_tests(takt_tests)
