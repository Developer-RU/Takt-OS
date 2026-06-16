# Copyright 2024-2026 Masyukov Pavel <p.masyukov@gmail.com>
# SPDX-License-Identifier: Apache-2.0
# TAKT OS — https://github.com/Developer-RU/Takt-OS

idf_component_register(
    SRCS "main.cpp"
    INCLUDE_DIRS "."
    REQUIRES kernel drivers middleware services takt_boot
)
