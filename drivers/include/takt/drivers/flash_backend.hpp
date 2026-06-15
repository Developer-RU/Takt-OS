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
