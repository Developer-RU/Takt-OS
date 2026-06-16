// Copyright 2024-2026 Masyukov Pavel <p.masyukov@gmail.com>
// SPDX-License-Identifier: Apache-2.0
//
// TAKT OS — https://github.com/Developer-RU/Takt-OS

#pragma once

#include "takt/imodule.hpp"
#include "takt/drivers/platform.hpp"

namespace takt::modules {

/// GPIO relay / actuator control module.
class RelayModule : public IModule {
public:
    RelayModule(int gpioPin = 26);

    bool init() override;
    void tick() override;
    void shutdown() override;
    const char* name() const override { return "Relay"; }
    ModuleType type() const override { return ModuleType::Static; }
    uint32_t budgetUs() const override { return 200; }

    void setRelay(bool on);
    bool isOn() const { return relayOn_; }

private:
    int gpioPin_;
    bool relayOn_ = false;
    bool pending_ = false;
    bool targetState_ = false;
};

} // namespace takt::modules
