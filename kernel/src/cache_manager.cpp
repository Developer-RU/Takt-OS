#include "takt/cache_manager.hpp"
#include "takt/storage_manager.hpp"
#include <cstring>
#include <algorithm>

namespace takt {

#ifdef TAKT_HOST_BUILD
CacheManager::CacheManager(uint8_t* pool, size_t poolSize, size_t lineSize)
    : pool_(pool), poolSize_(poolSize), lineSize_(lineSize) {}
#endif

CacheEntry* CacheManager::findLine(uint32_t flashOffset) {
    const uint32_t lineKey = flashOffset / static_cast<uint32_t>(lineSize_);
    for (size_t i = 0; i < entryCount_; ++i) {
        if (entries_[i].valid && entries_[i].key == lineKey) {
            ++hits_;
            entries_[i].lastAccess = hits_ + misses_;
            return &entries_[i];
        }
    }
  #ifdef TAKT_HOST_BUILD
    ++misses_;
  #else
    misses_++;
  #endif
    return nullptr;
}

CacheEntry* CacheManager::allocateLine(uint32_t flashOffset) {
    const uint32_t lineKey = flashOffset / static_cast<uint32_t>(lineSize_);
    if (entryCount_ < kMaxLines) {
        auto& e = entries_[entryCount_++];
        e = {lineKey, static_cast<uint32_t>(lineSize_), 0, false, true};
        return &e;
    }
    // Evict LRU
    size_t lruIdx = 0;
    uint32_t lruAccess = entries_[0].lastAccess;
    for (size_t i = 1; i < entryCount_; ++i) {
        if (entries_[i].lastAccess < lruAccess) {
            lruAccess = entries_[i].lastAccess;
            lruIdx = i;
        }
    }
    auto& e = entries_[lruIdx];
    if (e.dirty) flush();
    e = {lineKey, static_cast<uint32_t>(lineSize_), 0, false, true};
    return &e;
}

int CacheManager::read(uint32_t flashOffset, void* buf, size_t len) {
    auto* line = findLine(flashOffset);
    if (!line) {
        line = allocateLine(flashOffset);
        const uint32_t lineStart = line->key * static_cast<uint32_t>(lineSize_);
        uint8_t* poolLine = pool_ + (line - entries_) * lineSize_;
        if (StorageManager::instance().read(lineStart, poolLine, lineSize_) != 0) {
            return -1;
        }
    }
    const size_t offsetInLine = flashOffset % lineSize_;
    const size_t copyLen = std::min(len, lineSize_ - offsetInLine);
    std::memcpy(buf, pool_ + (line - entries_) * lineSize_ + offsetInLine, copyLen);
    return static_cast<int>(copyLen);
}

int CacheManager::write(uint32_t flashOffset, const void* buf, size_t len) {
    auto* line = findLine(flashOffset);
    if (!line) line = allocateLine(flashOffset);
    const size_t offsetInLine = flashOffset % lineSize_;
    const size_t copyLen = std::min(len, lineSize_ - offsetInLine);
    std::memcpy(pool_ + (line - entries_) * lineSize_ + offsetInLine, buf, copyLen);
    line->dirty = true;
    return static_cast<int>(copyLen);
}

int CacheManager::flush() {
    for (size_t i = 0; i < entryCount_; ++i) {
        if (entries_[i].valid && entries_[i].dirty) {
            const uint32_t lineStart = entries_[i].key * static_cast<uint32_t>(lineSize_);
            if (StorageManager::instance().write(lineStart,
                    pool_ + i * lineSize_, lineSize_) != 0) {
                return -1;
            }
            entries_[i].dirty = false;
        }
    }
    return 0;
}

void CacheManager::invalidate() {
    for (size_t i = 0; i < entryCount_; ++i) {
        entries_[i].valid = false;
        entries_[i].dirty = false;
    }
    entryCount_ = 0;
    hits_ = 0;
    misses_ = 0;
}

} // namespace takt
