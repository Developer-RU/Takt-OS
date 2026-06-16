// Copyright 2024-2026 Masyukov Pavel <p.masyukov@gmail.com>
// SPDX-License-Identifier: Apache-2.0
//
// TAKT OS — https://github.com/Developer-RU/Takt-OS

#include <gtest/gtest.h>
#include "takt/timer.hpp"
#include "takt/timer_manager.hpp"

namespace {

TEST(TimerTest, OneShotFires) {
    bool fired = false;
    takt::Timer timer(100, false);
    timer.onTimeout([&fired]() { fired = true; });
    timer.start();
    takt::TimerManager::instance().tick(50);
    EXPECT_FALSE(fired);
    takt::TimerManager::instance().tick(50);
    EXPECT_TRUE(fired);
}

TEST(TimerTest, RepeatResets) {
    int count = 0;
    takt::Timer timer(100, true);
    timer.onTimeout([&count]() { ++count; });
    timer.start();
    takt::TimerManager::instance().tick(100);
    takt::TimerManager::instance().tick(100);
    EXPECT_EQ(count, 2);
}

} // namespace
