#pragma once

#include <cstddef>
#include <cstdint>
#include <cstring>

namespace takt {

/// LRU cache entry metadata.
struct CacheEntry {
    uint32_t key     = 0;
    uint32_t size    = 0;
    uint32_t lastAccess = 0;
    bool     dirty   = false;
    bool     valid   = false;
};

/// In-RAM cache over flash storage.
///
/// Provides buffered read/write with write-back policy.
/// Used by FirmwareCache and application data caching.
class CacheManager {
public:
    /// @param pool      Pre-allocated memory pool for cache lines
    /// @param poolSize  Total pool size in bytes
    /// @param lineSize  Cache line size (must be power of 2)
  #ifdef TAKT_HOST_BUILD
    CacheManager(uint8_t* pool, size_t poolSize, size_t lineSize);
  #else
    CacheManager(uint8_t* pool, size_t poolSize, size_t lineSize)
        : pool_(pool), poolSize_(poolSize), lineSize_(lineSize) {}
  #endif

    /// Read through cache. Loads from flash on miss.
    int read(uint32_t flashOffset, void* buf, size_t len);

    /// Write to cache (marked dirty, flushed lazily or on flush()).
    int write(uint32_t flashOffset, const void* buf, size_t len);

    /// Flush all dirty lines to flash.
    int flush();

    /// Invalidate entire cache.
    void invalidate();

    /// Cache hit/miss statistics.
    uint32_t hits() const { return hits_; }
    uint32_t misses() const { return misses_; }

private:
    uint8_t*       pool_;
    size_t         poolSize_;
    size_t         lineSize_;
    uint32_t       hits_   = 0;
    uint32_t       misses_ = 0;

    static constexpr size_t kMaxLines = 64;
    CacheEntry entries_[kMaxLines]{};
    size_t     entryCount_ = 0;

    CacheEntry* findLine(uint32_t flashOffset);
    CacheEntry* allocateLine(uint32_t flashOffset);
};

} // namespace takt
