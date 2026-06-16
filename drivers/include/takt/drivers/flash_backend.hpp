// Copyright 2024-2026 Masyukov Pavel <p.masyukov@gmail.com>
// SPDX-License-Identifier: Apache-2.0
//
// TAKT OS — https://github.com/Developer-RU/Takt-OS

#pragma once

#include "takt/storage_manager.hpp"

namespace takt::drivers {

/// Create FlashBackend bound to an ESP-IDF partition label.
bool createPartitionBackend(FlashBackend& backend, const char* label);

/// Create FlashBackend for the raw storage partition (label "storage").
bool createStorageBackend(FlashBackend& backend);

/// Initialize StorageManager with ESP32 partition backend.
bool initStorageFromPartition(const char* label = "storage");

} // namespace takt::drivers
