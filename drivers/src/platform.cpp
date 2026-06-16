// Copyright 2024-2026 Masyukov Pavel <p.masyukov@gmail.com>
// SPDX-License-Identifier: Apache-2.0
//
// TAKT OS — https://github.com/Developer-RU/Takt-OS

#include "takt/drivers/platform.hpp"
#include "takt/drivers/flash_backend.hpp"
#include "takt/firmware_cache.hpp"
#include "takt/logger.hpp"

#ifdef TAKT_ESP32
#include "esp_system.h"
#include "esp_timer.h"
#include "esp_task_wdt.h"
#include "esp_partition.h"
#include "driver/gpio.h"
#else
#include <chrono>
#endif

namespace takt::drivers {

bool Platform::init() {
#ifdef TAKT_ESP32
    esp_task_wdt_config_t wdtCfg = {
        .timeout_ms = 10000,
        .idle_core_mask = 0,
        .trigger_panic = true,
    };
    esp_task_wdt_reconfigure(&wdtCfg);
    esp_task_wdt_add(nullptr);

    initStorageFromPartition("storage");
    const esp_partition_t* ota0 = esp_partition_find_first(
        ESP_PARTITION_TYPE_APP, ESP_PARTITION_SUBTYPE_APP_OTA_0, nullptr);
    const esp_partition_t* ota1 = esp_partition_find_first(
        ESP_PARTITION_TYPE_APP, ESP_PARTITION_SUBTYPE_APP_OTA_1, nullptr);
    if (ota0 && ota1) {
        FirmwareCache::instance().init(ota0->address, ota1->address, ota0->size);
    }
    return true;
#else
    return true;
#endif
}

void Platform::feedWatchdog() {
#ifdef TAKT_ESP32
    esp_task_wdt_reset();
#endif
}

uint32_t Platform::getResetReason() {
#ifdef TAKT_ESP32
    return static_cast<uint32_t>(esp_reset_reason());
#else
    return 0;
#endif
}

uint64_t Platform::getUptimeUs() {
#ifdef TAKT_ESP32
    return static_cast<uint64_t>(esp_timer_get_time());
#else
    using clock = std::chrono::steady_clock;
    static auto start = clock::now();
    return std::chrono::duration_cast<std::chrono::microseconds>(
        clock::now() - start).count();
#endif
}

bool Gpio::setMode(int pin, Mode mode) {
#ifdef TAKT_ESP32
    gpio_config_t cfg{};
    cfg.pin_bit_mask = (1ULL << pin);
    switch (mode) {
        case Mode::Input:         cfg.mode = GPIO_MODE_INPUT; break;
        case Mode::Output:        cfg.mode = GPIO_MODE_OUTPUT; break;
        case Mode::InputPullUp:   cfg.mode = GPIO_MODE_INPUT; cfg.pull_up_en = GPIO_PULLUP_ENABLE; break;
        case Mode::InputPullDown: cfg.mode = GPIO_MODE_INPUT; cfg.pull_down_en = GPIO_PULLDOWN_ENABLE; break;
    }
    return gpio_config(&cfg) == ESP_OK;
#else
    (void)pin; (void)mode;
    return true;
#endif
}

Gpio::Level Gpio::read(int pin) {
#ifdef TAKT_ESP32
    return gpio_get_level(static_cast<gpio_num_t>(pin)) ? Level::High : Level::Low;
#else
    (void)pin;
    return Level::Low;
#endif
}

void Gpio::write(int pin, Level level) {
#ifdef TAKT_ESP32
    gpio_set_level(static_cast<gpio_num_t>(pin), level == Level::High ? 1 : 0);
#else
    (void)pin; (void)level;
#endif
}

} // namespace takt::drivers
