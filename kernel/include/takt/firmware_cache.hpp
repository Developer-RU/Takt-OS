// Copyright 2024-2026 Masyukov Pavel <p.masyukov@gmail.com>
// SPDX-License-Identifier: Apache-2.0
//
// TAKT OS — https://github.com/Developer-RU/Takt-OS

#pragma once

#include "takt/storage_manager.hpp"
#include <cstddef>
#include <cstdint>

namespace takt {

/// Firmware image metadata stored in flash.
struct FirmwareHeader {
    uint32_t magic;          ///< 0x54414B54 ('TAKT')
    uint32_t version;        ///< Semantic version packed (major<<16 | minor<<8 | patch)
    uint32_t size;           ///< Image size in bytes
    uint32_t crc32;          ///< CRC32 of image body
    uint32_t timestamp;      ///< Unix timestamp of build
    uint8_t  slot;           ///< 0 = A, 1 = B
    uint8_t  flags;          ///< bit0=valid, bit1=bootable, bit2=pending_verify
    uint8_t  reserved[2];
};

static constexpr uint32_t kFirmwareMagic = 0x54414B54; // 'TAKT'

/// Dual-bank firmware cache for OTA with rollback support.
///
/// Manages two firmware slots (A/B) in dedicated flash regions.
/// The active slot runs the current firmware; the inactive slot
/// receives OTA updates. Rollback swaps slots atomically.
class FirmwareCache {
public:
    static FirmwareCache& instance() {
        static FirmwareCache cache;
        return cache;
    }

    /// Initialize with flash region offsets for slot A and slot B.
    bool init(uint32_t slotAOffset, uint32_t slotBOffset, uint32_t slotSize);

    /// Get the currently active slot (0=A, 1=B).
    uint8_t activeSlot() const { return activeSlot_; }

    /// Get the inactive slot available for OTA writes.
    uint8_t inactiveSlot() const { return activeSlot_ == 0 ? 1 : 0; }

    /// Begin writing a new firmware image to the inactive slot.
    bool beginWrite(uint32_t imageSize, uint32_t version);

    /// Write a chunk of firmware data.
    int writeChunk(const void* data, size_t len);

    /// Finalize write — compute CRC, mark slot valid.
    bool finalizeWrite();

    /// Verify firmware integrity in a slot.
    bool verify(uint8_t slot) const;

    /// Atomically switch boot slot (rollback-capable).
    bool activateSlot(uint8_t slot);

    /// Rollback to the previous slot.
    bool rollback();

    /// Read firmware header for a slot.
    bool getHeader(uint8_t slot, FirmwareHeader& header) const;

private:
    uint32_t slotOffset_[2]{};
    uint32_t slotSize_    = 0;
    uint8_t  activeSlot_  = 0;
    uint32_t writeOffset_ = 0;
    uint32_t writeCrc_    = 0;
    uint32_t writeImageSize_ = 0;
    bool     writing_     = false;

    uint32_t crc32(const void* data, size_t len) const;
};

} // namespace takt
