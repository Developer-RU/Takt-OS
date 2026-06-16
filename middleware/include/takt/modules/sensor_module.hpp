// Copyright 2024-2026 Masyukov Pavel <p.masyukov@gmail.com>
// SPDX-License-Identifier: Apache-2.0
//
// TAKT OS — https://github.com/Developer-RU/Takt-OS

#pragma once

#include "takt/imodule.hpp"

namespace takt::modules {

/// Static module: processes exactly one sensor sample per takt.
class SensorModule : public IModule {
public:
    bool init() override;
    void tick() override;
    void shutdown() override;
    const char* name() const override { return "Sensor"; }
    ModuleType type() const override { return ModuleType::Static; }
    uint32_t budgetUs() const override { return 2000; }

    float lastValue() const { return lastValue_; }

private:
    void processOneSample();
    float lastValue_ = 0.0f;
    uint32_t sampleCount_ = 0;
};

} // namespace takt::modules
