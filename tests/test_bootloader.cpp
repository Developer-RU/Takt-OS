#include <gtest/gtest.h>
#include "takt/bootloader.hpp"

TEST(BootloaderTest, BootConfigRoundTrip) {
    takt::boot::BootConfig cfg{};
    cfg.magic = takt::boot::kBootConfigMagic;
    cfg.mode = takt::boot::BootMode::OtaPending;
    cfg.bootCount = 2;
    cfg.otaSlot = 1;

    takt::boot::Bootloader::saveBootConfig(cfg);

    takt::boot::BootConfig loaded{};
    EXPECT_TRUE(takt::boot::Bootloader::loadBootConfig(loaded));
    EXPECT_EQ(loaded.bootCount, 2u);
    EXPECT_EQ(loaded.otaSlot, 1u);
}

TEST(BootloaderTest, MarkBootSuccessfulResetsCounter) {
    takt::boot::BootConfig cfg{};
    cfg.magic = takt::boot::kBootConfigMagic;
    cfg.bootCount = 5;
    takt::boot::Bootloader::saveBootConfig(cfg);
    takt::boot::Bootloader::markBootSuccessful();

    takt::boot::BootConfig loaded{};
    takt::boot::Bootloader::loadBootConfig(loaded);
    EXPECT_EQ(loaded.bootCount, 0u);
}
