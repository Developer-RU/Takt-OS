#pragma once

#include "takt/imodule.hpp"
#include <cstdint>
#include <functional>

namespace takt::services {

enum class OtaTransport : uint8_t {
    WiFi = 0,
    Ble  = 1,
};

using OtaProgressFn = std::function<void(uint32_t received, uint32_t total)>;

class OtaService : public IModule {
public:
    bool init() override;
    void tick() override;
    void shutdown() override;
    const char* name() const override { return "OTA"; }
    ModuleType type() const override { return ModuleType::Background; }
    bool hasWork() const override { return active_; }

    bool startUpdate(OtaTransport transport, uint32_t imageSize, uint32_t version);
    int feedChunk(const void* data, size_t len);
    bool completeUpdate();
    void onProgress(OtaProgressFn callback) { progressCb_ = std::move(callback); }
    bool rollback();

private:
    bool active_ = false;
    OtaTransport transport_ = OtaTransport::WiFi;
    OtaProgressFn progressCb_;
    uint32_t received_ = 0;
    uint32_t totalSize_ = 0;
};

} // namespace takt::services
