#include "takt/crc32.hpp"

namespace takt {

namespace {
uint32_t table[256];
bool tableInit = false;

void initTable() {
    if (tableInit) return;
    for (uint32_t i = 0; i < 256; ++i) {
        uint32_t crc = i;
        for (int j = 0; j < 8; ++j) {
            crc = (crc & 1) ? (crc >> 1) ^ 0xEDB88320 : crc >> 1;
        }
        table[i] = crc;
    }
    tableInit = true;
}
}

uint32_t Crc32::update(uint32_t crc, const void* data, size_t len) {
    initTable();
    const auto* bytes = static_cast<const uint8_t*>(data);
    for (size_t i = 0; i < len; ++i) {
        crc = table[(crc ^ bytes[i]) & 0xFF] ^ (crc >> 8);
    }
    return crc;
}

uint32_t Crc32::compute(const void* data, size_t len) {
    return finalize(update(initial(), data, len));
}

} // namespace takt
