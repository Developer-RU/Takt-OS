# Copyright 2024-2026 Masyukov Pavel <p.masyukov@gmail.com>
# SPDX-License-Identifier: Apache-2.0
# TAKT OS — https://github.com/Developer-RU/Takt-OS

cmake_minimum_required(VERSION 3.16)

set(TAKT_OS_ROOT "${CMAKE_CURRENT_LIST_DIR}/../..")
list(APPEND EXTRA_COMPONENT_DIRS
    "${TAKT_OS_ROOT}/kernel"
    "${TAKT_OS_ROOT}/drivers"
    "${TAKT_OS_ROOT}/middleware"
    "${TAKT_OS_ROOT}/services"
    "${TAKT_OS_ROOT}/recovery"
    "${TAKT_OS_ROOT}/takt_boot"
)

include($ENV{IDF_PATH}/tools/cmake/project.cmake)
project(recovery_app)
