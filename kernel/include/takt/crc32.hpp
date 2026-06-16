// Copyright 2024-2026 Masyukov Pavel <p.masyukov@gmail.com>
// SPDX-License-Identifier: Apache-2.0
//
// TAKT OS — https://github.com/Developer-RU/Takt-OS

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
