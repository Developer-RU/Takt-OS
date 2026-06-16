// Copyright 2024-2026 Masyukov Pavel <p.masyukov@gmail.com>
// SPDX-License-Identifier: Apache-2.0
//
// TAKT OS — https://github.com/Developer-RU/Takt-OS

#pragma once

#include "takt/imodule.hpp"

namespace takt::modules {

/// Background module: processes BLE events when available.
class BleModule : public IModule {
public:
    bool init() override;
    void tick() override;
    void shutdown() override;
    const char* name() const override { return "BLE"; }
    ModuleType type() const override { return ModuleType::Background; }
    bool hasWork() const override { return eventPending_; }

private:
    bool eventPending_ = false;
    void processEvents();
};

} // namespace takt::modules
