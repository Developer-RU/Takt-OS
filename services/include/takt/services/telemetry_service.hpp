#pragma once

#include "takt/imodule.hpp"
#include "takt/cache_manager.hpp"
#include <cstddef>
#include <cstdint>

namespace takt::services {

struct TelemetryRecord {
    uint32_t timestamp;
    uint32_t sensorValue;
    uint8_t  relayState;
    uint8_t  reserved[3];
};

/// Batch telemetry upload service using CacheManager.
class TelemetryService : public IModule {
public:
    TelemetryService(uint8_t* cachePool, size_t poolSize);

    bool init() override;
    void tick() override;
    void shutdown() override;
    const char* name() const override { return "Telemetry"; }
    ModuleType type() const override { return ModuleType::Background; }
    bool hasWork() const override { return flushPending_; }

    void record(const TelemetryRecord& rec);
    void setUploadIntervalMs(uint32_t ms) { uploadIntervalMs_ = ms; }

private:
    CacheManager cache_;
    uint32_t uploadIntervalMs_ = 60000;
    uint32_t lastUploadMs_ = 0;
    bool flushPending_ = false;
    static constexpr size_t kMaxRecords = 64;
    TelemetryRecord buffer_[kMaxRecords]{};
    size_t recordCount_ = 0;

    void flushToFlash();
    void uploadBatch();
};

} // namespace takt::services
