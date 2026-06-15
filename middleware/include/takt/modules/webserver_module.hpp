#pragma once

#include "takt/imodule.hpp"

namespace takt::modules {

/// HTTP web server for diagnostics and control.
class WebServerModule : public IModule {
public:
    bool init() override;
    void tick() override;
    void shutdown() override;
    const char* name() const override { return "WebServer"; }
    ModuleType type() const override { return ModuleType::Background; }
    bool hasWork() const override { return false; }

    void setPort(uint16_t port) { port_ = port; }

private:
    uint16_t port_ = 80;
    bool running_ = false;
};

} // namespace takt::modules
