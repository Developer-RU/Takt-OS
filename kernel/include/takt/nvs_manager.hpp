// Copyright 2024-2026 Masyukov Pavel <p.masyukov@gmail.com>
// SPDX-License-Identifier: Apache-2.0
//
// TAKT OS — https://github.com/Developer-RU/Takt-OS

#pragma once

#include <cstddef>
#include <cstdint>

namespace takt {

/// NVS entry metadata with versioning and integrity.
struct NvsEntry {
    char     key[32];
    uint16_t version;
    uint16_t dataSize;
    uint32_t crc32;
};

static constexpr uint16_t kNvsBackupVersion = 0xFFFE;

/// NVS Manager — key-value persistent storage with corruption protection.
///
/// Features:
///   - Typed key-value storage (blob, u8, u16, u32, string)
///   - Per-key versioning
///   - Automatic backup copy in a dedicated backup sector
///   - CRC32 integrity check on read
///   - Recovery from backup on primary corruption
class NvsManager {
public:
    static NvsManager& instance() {
        static NvsManager mgr;
        return mgr;
    }

    /// Initialize NVS subsystem. Creates namespace if missing.
    bool init(const char* namespaceName = "takt");

    /// Store a blob value with version tag.
    bool setBlob(const char* key, const void* data, size_t len, uint16_t version = 1);

    /// Read a blob value. Returns bytes read or -1 on error.
    int getBlob(const char* key, void* data, size_t maxLen, uint16_t* version = nullptr) const;

    /// Store integer values.
    bool setU8(const char* key, uint8_t value, uint16_t version = 1);
    bool setU16(const char* key, uint16_t value, uint16_t version = 1);
    bool setU32(const char* key, uint32_t value, uint16_t version = 1);

    /// Read integer values. Returns false if key not found or corrupt.
    bool getU8(const char* key, uint8_t& value) const;
    bool getU16(const char* key, uint16_t& value) const;
    bool getU32(const char* key, uint32_t& value) const;

    /// Store a null-terminated string.
    bool setString(const char* key, const char* value, uint16_t version = 1);
    bool getString(const char* key, char* buf, size_t maxLen) const;

    /// Delete a key.
    bool remove(const char* key);

    /// Erase entire namespace.
    bool eraseAll();

    /// Force backup of all entries to backup sector.
    bool backup();

    /// Restore from backup sector (used after corruption detection).
    bool restoreFromBackup();

    /// Verify integrity of all stored entries.
    bool verifyIntegrity() const;

private:
    char namespace_[16]{};
    bool initialized_ = false;

    uint32_t computeCrc(const void* data, size_t len) const;
    bool writeEntry(const char* key, const void* data, size_t len, uint16_t version);
    bool readEntry(const char* key, void* data, size_t maxLen, uint16_t* version) const;
};

} // namespace takt
