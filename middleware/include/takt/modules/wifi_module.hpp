#pragma once

#include "takt/imodule.hpp"
#include "takt/timer.hpp"
#include <cstring>

namespace takt::modules {

struct WiFiConfig {
    char ssid[32] = "TAKT-OS";
    char password[64] = "";
};

/// Background WiFi module with esp_wifi integration.
class WiFiModule : public IModule {
public:
    bool init() override;
    void tick() override;
    void shutdown() override;
    const char* name() const override { return "WiFi"; }
    ModuleType type() const override { return ModuleType::Background; }
    bool hasWork() const override { return reconnectPending_ || eventPending_; }

    bool isConnected() const { return connected_; }
    void setConfig(const WiFiConfig& cfg);

private:
    bool connected_ = false;
    bool reconnectPending_ = true;
    bool eventPending_ = false;
    bool initialized_ = false;
    WiFiConfig config_{};
    Timer reconnectTimer_{5000, true};

    void processEvents();
    bool connect();
};

} // namespace takt::modules
