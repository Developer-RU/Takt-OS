// Copyright 2024-2026 Masyukov Pavel <p.masyukov@gmail.com>
// SPDX-License-Identifier: Apache-2.0
//
// TAKT OS — https://github.com/Developer-RU/Takt-OS

#include "takt/bootloader.hpp"
#include "takt/firmware_cache.hpp"
#include "takt/nvs_manager.hpp"
#include "takt/logger.hpp"

#ifdef TAKT_ESP32
#include "esp_log.h"
#include "esp_system.h"
#include "esp_ota_ops.h"
#include "esp_app_format.h"
#include "driver/gpio.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#else
#include <cstdio>
#include <cstdlib>
#endif

namespace takt::boot {

namespace {
constexpr const char* TAG = "Bootloader";

#ifdef TAKT_ESP32
RTC_NOINIT_ATTR BootConfig rtcBootConfig;
#else
static BootConfig gHostBootConfig{};
#endif
}

uint32_t Bootloader::crc32Config(const void* data, size_t len) {
    const auto* b = static_cast<const uint8_t*>(data);
    uint32_t crc = 0xFFFFFFFF;
    for (size_t i = 0; i < len; ++i) {
        crc ^= b[i];
        for (int j = 0; j < 8; ++j) crc = (crc >> 1) ^ (crc & 1 ? 0xEDB88320 : 0);
    }
    return crc ^ 0xFFFFFFFF;
}

bool Bootloader::loadBootConfig(BootConfig& config) {
#ifdef TAKT_ESP32
    config = rtcBootConfig;
#else
    config = gHostBootConfig;
#endif
    if (config.magic != kBootConfigMagic) {
        config = {};
        config.magic = kBootConfigMagic;
        config.mode = BootMode::Normal;
        return false;
    }
    const uint32_t stored = config.crc32;
    config.crc32 = 0;
    return crc32Config(&config, sizeof(config)) == stored;
}

void Bootloader::saveBootConfig(const BootConfig& config) {
    BootConfig cfg = config;
    cfg.crc32 = 0;
    cfg.crc32 = crc32Config(&cfg, sizeof(cfg));
#ifdef TAKT_ESP32
    rtcBootConfig = cfg;
#else
    gHostBootConfig = cfg;
#endif
}

BootMode Bootloader::determineBootMode() {
    BootConfig config{};
    loadBootConfig(config);

#ifdef TAKT_ESP32
    gpio_set_direction(GPIO_NUM_0, GPIO_MODE_INPUT);
    gpio_set_pull_mode(GPIO_NUM_0, GPIO_PULLUP_ONLY);
    if (gpio_get_level(GPIO_NUM_0) == 0) {
        return BootMode::Recovery;
    }
#endif

    if (config.bootCount > 3) return BootMode::Recovery;
    if (config.mode == BootMode::FactoryReset) return BootMode::FactoryReset;
    if (config.mode == BootMode::OtaPending) return BootMode::OtaPending;
    if (!validateFirmware(0) && !validateFirmware(1)) return BootMode::Emergency;
    return BootMode::Normal;
}

bool Bootloader::validateFirmware(uint8_t slot) {
#ifdef TAKT_ESP32
    const char* label = (slot == 0) ? "ota_0" : "ota_1";
    const esp_partition_t* part = esp_partition_find_first(
        ESP_PARTITION_TYPE_APP, ESP_PARTITION_SUBTYPE_APP_OTA_MIN, label);
    if (!part) return false;

    esp_app_desc_t desc{};
    if (esp_ota_get_partition_description(part, &desc) != ESP_OK) return false;
    return desc.magic_word == ESP_APP_DESC_MAGIC_WORD;
#else
    return FirmwareCache::instance().verify(slot);
#endif
}

void Bootloader::jumpToFirmware(uint32_t flashOffset) {
#ifdef TAKT_ESP32
    (void)flashOffset;
    const esp_partition_t* part = esp_ota_get_next_update_partition(nullptr);
    if (!part) part = esp_ota_get_running_partition();
    if (part) {
        esp_ota_set_boot_partition(part);
        TAKT_LOGI(TAG, "Rebooting to partition %s", part->label);
        esp_restart();
    }
    abort();
#else
    std::printf("[Bootloader] Jump to 0x%08X\n", flashOffset);
    std::exit(0);
#endif
}

void Bootloader::enterRecovery() {
    TAKT_LOGW(TAG, "Entering recovery mode");
#ifdef TAKT_ESP32
    const esp_partition_t* recovery = esp_partition_find_first(
        ESP_PARTITION_TYPE_APP, ESP_PARTITION_SUBTYPE_APP_TEST, "recovery");
    if (recovery) {
        esp_ota_set_boot_partition(recovery);
        esp_restart();
    }
#endif
}

void Bootloader::enterEmergency() {
    TAKT_LOGE(TAG, "EMERGENCY MODE");
#ifdef TAKT_ESP32
    while (true) { vTaskDelay(pdMS_TO_TICKS(1000)); }
#endif
}

void Bootloader::markBootSuccessful() {
    BootConfig config{};
    loadBootConfig(config);
    config.bootCount = 0;
    config.mode = BootMode::Normal;
    saveBootConfig(config);
}

void Bootloader::requestOtaBoot(uint8_t slot) {
    BootConfig config{};
    loadBootConfig(config);
    config.mode = BootMode::OtaPending;
    config.otaSlot = slot;
    saveBootConfig(config);
}

void Bootloader::entry() {
    TAKT_LOGI(TAG, "TAKT Bootloader v0.2.0");

    BootConfig config{};
    loadBootConfig(config);
    config.bootCount++;
    saveBootConfig(config);

    switch (determineBootMode()) {
        case BootMode::Normal:
            jumpToFirmware(0);
            break;
        case BootMode::Recovery:
            enterRecovery();
            break;
        case BootMode::FactoryReset:
            NvsManager::instance().eraseAll();
            enterRecovery();
            break;
        case BootMode::OtaPending:
            jumpToFirmware(config.otaSlot == 0 ? 0 : 0);
            break;
        case BootMode::Emergency:
            enterEmergency();
            break;
    }
}

} // namespace takt::boot
