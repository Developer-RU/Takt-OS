# Copyright 2024-2026 Masyukov Pavel <p.masyukov@gmail.com>
# SPDX-License-Identifier: Apache-2.0
# TAKT OS — https://github.com/Developer-RU/Takt-OS

# Demo Controller Example
if(DEFINED ESP_PLATFORM)
    add_subdirectory(demo_controller)
    add_subdirectory(recovery_app)
endif()
