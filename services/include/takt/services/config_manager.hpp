#pragma once

#include "takt/imodule.hpp"
#include <functional>

namespace takt::services {

using ConfigChangeCallback = std::function<void(const char* key)>;

/// Remote configuration manager (MQTT + NVS).
class ConfigManager : public IModule {
public:
    bool init() override;
    void tick() override;
    void shutdown() override;
    const char* name() const override { return "ConfigMgr"; }
    ModuleType type() const override { return ModuleType::Background; }
    bool hasWork() const override { return applyPending_; }

    bool setString(const char* key, const char* value);
    bool getString(const char* key, char* buf, size_t maxLen) const;
    void onChange(ConfigChangeCallback cb) { changeCb_ = std::move(cb); }
    void applyRemoteConfig(const char* json);

private:
    bool applyPending_ = false;
    ConfigChangeCallback changeCb_;
    char pendingJson_[512]{};

    void applyPending();
};

} // namespace takt::services
