// Copyright 2024-2026 Masyukov Pavel <p.masyukov@gmail.com>
// SPDX-License-Identifier: Apache-2.0
//
// TAKT OS — https://github.com/Developer-RU/Takt-OS

#include "takt/services/ota_service.hpp"
#include "takt/firmware_cache.hpp"
#include "takt/recovery_manager.hpp"
#include "takt/bootloader.hpp"
#include "takt/event_bus.hpp"
#include "takt/logger.hpp"

#ifdef TAKT_ESP32
#include "esp_system.h"
#endif

namespace takt::services {

namespace {
constexpr const char* TAG = "OTA";
}

bool OtaService::init() {
    TAKT_LOGI(TAG, "OTA service init");
    return true;
}

bool OtaService::startUpdate(OtaTransport transport, uint32_t imageSize, uint32_t version) {
    transport_ = transport;
    totalSize_ = imageSize;
    received_ = 0;

    recovery::RecoveryChannel ch = recovery::RecoveryChannel::WiFi;
    if (transport == OtaTransport::Ble) ch = recovery::RecoveryChannel::Ble;

    auto& recovery = recovery::RecoveryManager::instance();
    recovery.init(ch);
    recovery.onProgress([this](uint32_t rx, uint32_t total, auto) {
        received_ = rx;
        totalSize_ = total;
        if (progressCb_) progressCb_(rx, total);
        EventBus::instance().publish(Event::OtaProgress, rx, total);
    });

    if (!FirmwareCache::instance().beginWrite(imageSize, version)) return false;
    recovery.startDfuListener();
    active_ = true;
    EventBus::instance().publish(Event::OtaStart, imageSize, static_cast<uint32_t>(transport));
    TAKT_LOGI(TAG, "OTA started transport=%u size=%u", static_cast<uint8_t>(transport), imageSize);
    return true;
}

int OtaService::feedChunk(const void* data, size_t len) {
    if (!active_) return -1;
    const int n = recovery::RecoveryManager::instance().receiveChunk(data, len);
    if (n > 0) received_ += static_cast<uint32_t>(n);
    return n;
}

bool OtaService::completeUpdate() {
    if (!active_) return false;
    if (!recovery::RecoveryManager::instance().finalizeDfu()) {
        EventBus::instance().publish(Event::OtaFailed);
        active_ = false;
        return false;
    }
    active_ = false;
    return true;
}

void OtaService::tick() {
    if (!active_) return;
    recovery::RecoveryManager::instance().tick();
}

bool OtaService::rollback() {
    active_ = false;
    const bool ok = FirmwareCache::instance().rollback();
    EventBus::instance().publish(Event::OtaRollback);
#ifdef TAKT_ESP32
    if (ok) esp_restart();
#endif
    return ok;
}

void OtaService::shutdown() {
    active_ = false;
    recovery::RecoveryManager::instance().abort();
    TAKT_LOGI(TAG, "OTA service shutdown");
}

} // namespace takt::services
