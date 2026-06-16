// Copyright 2024-2026 Masyukov Pavel <p.masyukov@gmail.com>
// SPDX-License-Identifier: Apache-2.0
//
// TAKT OS — https://github.com/Developer-RU/Takt-OS

#include "takt/firmware_cache.hpp"
#include "takt/storage_manager.hpp"
#include "takt/crc32.hpp"
#include "takt/logger.hpp"
#include <cstring>

#ifdef TAKT_ESP32
#include "esp_ota_ops.h"
#include "esp_app_format.h"
#endif

namespace takt {

namespace {
constexpr const char* TAG = "FwCache";
static constexpr size_t kVerifyChunk = 4096;
static constexpr uint32_t kHeaderReserved = 4096; // one flash sector for metadata
}

bool FirmwareCache::init(uint32_t slotAOffset, uint32_t slotBOffset, uint32_t slotSize) {
    slotOffset_[0] = slotAOffset;
    slotOffset_[1] = slotBOffset;
    slotSize_ = slotSize;

#ifdef TAKT_ESP32
    const esp_partition_t* running = esp_ota_get_running_partition();
    if (running) {
        activeSlot_ = (std::strcmp(running->label, "ota_0") == 0) ? 0 : 1;
        TAKT_LOGI(TAG, "ESP-OTA running partition: %s", running->label);
        return true;
    }
#endif

    FirmwareHeader hdrA{}, hdrB{};
    const bool validA = getHeader(0, hdrA) && (hdrA.flags & 0x01);
    const bool validB = getHeader(1, hdrB) && (hdrB.flags & 0x01);
    activeSlot_ = validA ? 0 : (validB ? 1 : 0);
    return true;
}

bool FirmwareCache::beginWrite(uint32_t imageSize, uint32_t version) {
    if (imageSize > slotSize_ - kHeaderReserved) {
        TAKT_LOGE(TAG, "Image too large: %u", imageSize);
        return false;
    }
    const uint8_t slot = inactiveSlot();
    writeOffset_ = kHeaderReserved;
    writeCrc_ = Crc32::initial();
    writeImageSize_ = imageSize;
    writeVersion_ = version;
    writing_ = true;

    TAKT_LOGI(TAG, "Begin write slot %c size=%u", slot == 0 ? 'A' : 'B', imageSize);
    return true;
}

int FirmwareCache::writeChunk(const void* data, size_t len) {
    if (!writing_) return -1;
    const uint8_t slot = inactiveSlot();
    const uint32_t offset = slotOffset_[slot] + writeOffset_;
    if (StorageManager::instance().write(offset, data, len) < 0) return -1;
    writeCrc_ = Crc32::update(writeCrc_, data, len);
    writeOffset_ += static_cast<uint32_t>(len);
    return static_cast<int>(len);
}

bool FirmwareCache::finalizeWrite() {
    if (!writing_) return false;
    const uint8_t slot = inactiveSlot();
    const uint32_t finalCrc = Crc32::finalize(writeCrc_);

    FirmwareHeader hdr{};
    hdr.magic = kFirmwareMagic;
    hdr.version = writeVersion_;
    hdr.size = writeImageSize_;
    hdr.slot = slot;
    hdr.crc32 = finalCrc;
    hdr.flags = 0x01;
    StorageManager::instance().write(slotOffset_[slot], &hdr, sizeof(hdr));
    writing_ = false;
    TAKT_LOGI(TAG, "Finalized slot %c CRC=0x%08X", slot == 0 ? 'A' : 'B', finalCrc);
    return verify(slot);
}

bool FirmwareCache::verify(uint8_t slot) const {
    if (slot > 1) return false;
    FirmwareHeader hdr{};
    if (!getHeader(slot, hdr)) return false;
    if (hdr.magic != kFirmwareMagic || !(hdr.flags & 0x01)) return false;

    uint32_t crc = Crc32::initial();
    uint32_t remaining = hdr.size;
    uint32_t offset = slotOffset_[slot] + kHeaderReserved;
    uint8_t buf[kVerifyChunk];

    while (remaining > 0) {
        const size_t chunk = remaining > kVerifyChunk ? kVerifyChunk : remaining;
        if (StorageManager::instance().read(offset, buf, chunk) != static_cast<int>(chunk)) {
            TAKT_LOGE(TAG, "Verify read failed at 0x%08X", offset);
            return false;
        }
        crc = Crc32::update(crc, buf, chunk);
        offset += static_cast<uint32_t>(chunk);
        remaining -= static_cast<uint32_t>(chunk);
    }
    const uint32_t computed = Crc32::finalize(crc);
    if (computed != hdr.crc32) {
        TAKT_LOGE(TAG, "CRC mismatch slot %u: expected 0x%08X got 0x%08X",
                  slot, hdr.crc32, computed);
        return false;
    }
    return true;
}

bool FirmwareCache::activateSlot(uint8_t slot) {
    if (slot > 1 || !verify(slot)) return false;

#ifdef TAKT_ESP32
    const char* label = (slot == 0) ? "ota_0" : "ota_1";
    const esp_partition_t* part = esp_partition_find_first(
        ESP_PARTITION_TYPE_APP, ESP_PARTITION_SUBTYPE_APP_OTA_MIN, label);
    if (part && esp_ota_set_boot_partition(part) == ESP_OK) {
        activeSlot_ = slot;
        TAKT_LOGI(TAG, "ESP-OTA boot partition set to %s", label);
        return true;
    }
#endif

    FirmwareHeader hdr{};
    getHeader(slot, hdr);
    hdr.flags |= 0x02;
    StorageManager::instance().write(slotOffset_[slot], &hdr, sizeof(hdr));
    activeSlot_ = slot;
    return true;
}

bool FirmwareCache::rollback() {
    const uint8_t prev = inactiveSlot();
    if (!verify(prev)) return false;
    return activateSlot(prev);
}

bool FirmwareCache::getHeader(uint8_t slot, FirmwareHeader& header) const {
    if (slot > 1) return false;
    if (StorageManager::instance().read(slotOffset_[slot], &header, sizeof(header)) != static_cast<int>(sizeof(header))) {
        return false;
    }
    return header.magic == kFirmwareMagic;
}

uint32_t FirmwareCache::crc32(const void* data, size_t len) const {
    return Crc32::compute(data, len);
}

} // namespace takt
