#pragma once

#include "takt/imodule.hpp"
#include <cstring>

namespace takt::modules {

/// MQTT client module (esp_mqtt).
class MqttModule : public IModule {
public:
    bool init() override;
    void tick() override;
    void shutdown() override;
    const char* name() const override { return "MQTT"; }
    ModuleType type() const override { return ModuleType::Background; }
    bool hasWork() const override { return msgPending_; }

    bool publish(const char* topic, const char* payload);
    bool subscribe(const char* topic);
    void setBroker(const char* uri);

private:
    bool msgPending_ = false;
    bool connected_ = false;
    char brokerUri_[128] = "mqtt://broker.hivemq.com:1883";
    char pendingTopic_[64]{};
    char pendingPayload_[256]{};
};

} // namespace takt::modules
