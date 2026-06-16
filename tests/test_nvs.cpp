// Copyright 2024-2026 Masyukov Pavel <p.masyukov@gmail.com>
// SPDX-License-Identifier: Apache-2.0
//
// TAKT OS — https://github.com/Developer-RU/Takt-OS

#include <gtest/gtest.h>
#include "takt/nvs_manager.hpp"

TEST(NvsTest, InitSucceeds) {
    EXPECT_TRUE(takt::NvsManager::instance().init("test"));
}

TEST(NvsTest, SetGetU32) {
    auto& nvs = takt::NvsManager::instance();
    nvs.init("test");
    EXPECT_TRUE(nvs.setU32("counter", 42));
    uint32_t val = 0;
    EXPECT_TRUE(nvs.getU32("counter", val));
    EXPECT_EQ(val, 42u);
}
