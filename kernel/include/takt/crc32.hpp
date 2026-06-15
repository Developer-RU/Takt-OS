#pragma once

#include <cstddef>
#include <cstdint>

namespace takt {

/// Shared CRC32 (IEEE 802.3 / Ethernet polynomial).
class Crc32 {
public:
    static uint32_t compute(const void* data, size_t len);
    static uint32_t update(uint32_t crc, const void* data, size_t len);
    static uint32_t finalize(uint32_t crc) { return crc ^ 0xFFFFFFFF; }
    static uint32_t initial() { return 0xFFFFFFFF; }
};

} // namespace takt
