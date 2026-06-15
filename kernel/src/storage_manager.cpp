#include "takt/storage_manager.hpp"
#include "takt/logger.hpp"
#include <cstring>

namespace takt {

namespace {
constexpr const char* TAG = "Storage";
}

bool StorageManager::init(const FlashBackend& backend) {
    if (!backend.read || !backend.write || !backend.erase) {
        TAKT_LOGE(TAG, "Invalid flash backend");
        return false;
    }
    backend_ = backend;
    TAKT_LOGI(TAG, "Storage initialized: %u bytes, sector %u",
              backend.totalSize, backend.sectorSize);
    return true;
}

int StorageManager::read(uint32_t offset, void* buf, size_t len) const {
    if (!backend_.read) return -1;
    if (offset + len > backend_.totalSize) return -1;
    return backend_.read(offset, buf, len);
}

int StorageManager::write(uint32_t offset, const void* buf, size_t len) {
    if (!backend_.write) return -1;
    if (offset + len > backend_.totalSize) return -1;
    const uint32_t sector = backend_.sectorSize;
    const uint32_t startSector = offset / sector;
    const uint32_t endSector = (offset + len - 1) / sector;
    for (uint32_t s = startSector; s <= endSector; ++s) {
        if (backend_.erase(s * sector, sector) != 0) {
            TAKT_LOGE(TAG, "Erase failed at offset 0x%08X", s * sector);
            return -1;
        }
    }
    return backend_.write(offset, buf, len);
}

int StorageManager::eraseSector(uint32_t offset) {
    if (!backend_.erase) return -1;
    return backend_.erase(offset, backend_.sectorSize);
}

bool StorageManager::registerRegion(const char* name, uint32_t offset, uint32_t size) {
    if (regionCount_ >= kMaxRegions) return false;
    auto& r = regions_[regionCount_];
    std::strncpy(r.name, name, sizeof(r.name) - 1);
    r.region = {offset, size, r.name};
    r.active = true;
    ++regionCount_;
    TAKT_LOGI(TAG, "Region '%s': offset=0x%08X size=%u", name, offset, size);
    return true;
}

const FlashRegion* StorageManager::getRegion(const char* name) const {
    for (size_t i = 0; i < regionCount_; ++i) {
        if (regions_[i].active && std::strcmp(regions_[i].name, name) == 0) {
            return &regions_[i].region;
        }
    }
    return nullptr;
}

} // namespace takt
