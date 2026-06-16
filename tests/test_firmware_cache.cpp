// Copyright 2024-2026 Masyukov Pavel <p.masyukov@gmail.com>
// SPDX-License-Identifier: Apache-2.0
//
// TAKT OS — https://github.com/Developer-RU/Takt-OS

#include <gtest/gtest.h>
#include <cstring>
#include "takt/crc32.hpp"
#include "takt/firmware_cache.hpp"
#include "takt/storage_manager.hpp"

namespace {

class MemBackend {
public:
    static constexpr size_t kSize = 64 * 1024;
    uint8_t mem[kSize]{};

    static bool setup(takt::FlashBackend& b, MemBackend& ctx) {
        b.totalSize = kSize;
        b.sectorSize = 4096;
        b.read = [&ctx](uint32_t off, void* buf, size_t len) {
            if (off + len > kSize) return -1;
            std::memcpy(buf, ctx.mem + off, len);
            return static_cast<int>(len);
        };
        b.write = [&ctx](uint32_t off, const void* buf, size_t len) {
            if (off + len > kSize) return -1;
            std::memcpy(ctx.mem + off, buf, len);
            return static_cast<int>(len);
        };
        b.erase = [&ctx](uint32_t off, size_t len) {
            if (off + len > kSize) return -1;
            std::memset(ctx.mem + off, 0xFF, len);
            return 0;
        };
        return true;
    }
};

TEST(FirmwareCacheTest, WriteVerifyCrc) {
    MemBackend mem{};
    takt::FlashBackend backend{};
    MemBackend::setup(backend, mem);
    takt::StorageManager::instance().init(backend);

    auto& fc = takt::FirmwareCache::instance();
    fc.init(0x1000, 0x8000, 0x6000);

    const char data[] = "TAKT firmware image test payload";
    ASSERT_TRUE(fc.beginWrite(sizeof(data), 0x00010000));
    ASSERT_GT(fc.writeChunk(data, sizeof(data)), 0);
    ASSERT_TRUE(fc.finalizeWrite());
    ASSERT_TRUE(fc.verify(fc.inactiveSlot()));
}

TEST(Crc32Test, KnownValue) {
    const char* s = "123456789";
    EXPECT_EQ(takt::Crc32::compute(s, 9), 0xCBF43926u);
}

} // namespace
