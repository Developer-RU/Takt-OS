// Copyright 2024-2026 Masyukov Pavel <p.masyukov@gmail.com>
// SPDX-License-Identifier: Apache-2.0
//
// TAKT OS — https://github.com/Developer-RU/Takt-OS

#include "takt/services/telemetry_service.hpp"
#include "takt/storage_manager.hpp"
#include "takt/drivers/platform.hpp"
#include "takt/logger.hpp"

namespace takt::services {

namespace {
constexpr const char* TAG = "Telemetry";
constexpr uint32_t kTelemetryFlashOffset = 0;
}

TelemetryService::TelemetryService(uint8_t* cachePool, size_t poolSize)
    : cache_(cachePool, poolSize, 256) {}

bool TelemetryService::init() {
    TAKT_LOGI(TAG, "Telemetry service init");
    return true;
}

void TelemetryService::record(const TelemetryRecord& rec) {
    if (recordCount_ < kMaxRecords) {
        buffer_[recordCount_++] = rec;
    }
    flushPending_ = true;
}

void TelemetryService::flushToFlash() {
    if (recordCount_ == 0) return;
    cache_.write(kTelemetryFlashOffset, buffer_, recordCount_ * sizeof(TelemetryRecord));
    cache_.flush();
    recordCount_ = 0;
}

void TelemetryService::uploadBatch() {
    flushToFlash();
    TAKT_LOGI(TAG, "Telemetry batch uploaded");
    flushPending_ = false;
}

void TelemetryService::tick() {
    const uint32_t now = static_cast<uint32_t>(drivers::Platform::getUptimeUs() / 1000);
    if (flushPending_ && (now - lastUploadMs_ >= uploadIntervalMs_)) {
        uploadBatch();
        lastUploadMs_ = now;
    }
}

void TelemetryService::shutdown() {
    flushToFlash();
    TAKT_LOGI(TAG, "Telemetry shutdown");
}

} // namespace takt::services
