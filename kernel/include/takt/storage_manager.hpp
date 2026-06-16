// Copyright 2024-2026 Masyukov Pavel <p.masyukov@gmail.com>
// SPDX-License-Identifier: Apache-2.0
//
// TAKT OS — https://github.com/Developer-RU/Takt-OS

#pragma once

#include <cstddef>
#include <cstdint>
#include <functional>

namespace takt {

/// Flash region descriptor for named partitions.
struct FlashRegion {
    uint32_t offset;
    uint32_t size;
    const char* label;
};

/// Opaque flash read/write operations — platform-specific backend.
struct FlashBackend {
    std::function<int(uint32_t offset, void* buf, size_t len)> read;
    std::function<int(uint32_t offset, const void* buf, size_t len)> write;
    std::function<int(uint32_t offset, size_t len)> erase;
    uint32_t totalSize = 0;
    uint32_t sectorSize = 4096;
};

/// Raw flash storage manager.
///
/// Manages flash partitions as arbitrary byte-addressable regions.
/// Does NOT use SPIFFS/LittleFS — direct sector-level access.
class StorageManager {
public:
    static StorageManager& instance() {
        static StorageManager mgr;
        return mgr;
    }

    /// Initialize with platform flash backend.
    bool init(const FlashBackend& backend);

    /// Read bytes from flash at absolute offset.
    int read(uint32_t offset, void* buf, size_t len) const;

    /// Write bytes (erase-before-write handled internally).
    int write(uint32_t offset, const void* buf, size_t len);

    /// Erase a flash sector.
    int eraseSector(uint32_t offset);

    /// Register a named region for bounds-checked access.
    bool registerRegion(const char* name, uint32_t offset, uint32_t size);

    /// Get region info by name.
    const FlashRegion* getRegion(const char* name) const;

    uint32_t totalSize() const { return backend_.totalSize; }

private:
    FlashBackend backend_{};
    static constexpr size_t kMaxRegions = 16;
    struct NamedRegion {
        char         name[32];
        FlashRegion  region{};
        bool         active = false;
    };
    NamedRegion regions_[kMaxRegions]{};
    size_t      regionCount_ = 0;
};

} // namespace takt
