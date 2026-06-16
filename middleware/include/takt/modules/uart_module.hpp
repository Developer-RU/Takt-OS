// Copyright 2024-2026 Masyukov Pavel <p.masyukov@gmail.com>
// SPDX-License-Identifier: Apache-2.0
//
// TAKT OS — https://github.com/Developer-RU/Takt-OS

#pragma once

#include "takt/imodule.hpp"
#include <cstddef>
#include <cstdint>

namespace takt::modules {

/// Static module: reads a fixed number of bytes from UART each takt.
class UartModule : public IModule {
public:
    UartModule(int port = 0, size_t bytesPerTick = 16);

    bool init() override;
    void tick() override;
    void shutdown() override;
    const char* name() const override { return "UART"; }
    ModuleType type() const override { return ModuleType::Static; }
    uint32_t budgetUs() const override { return 500; }

    size_t bytesAvailable() const { return rxCount_; }

private:
    int    port_;
    size_t bytesPerTick_;
    size_t rxCount_ = 0;
    static constexpr size_t kRxBufSize = 256;
    uint8_t rxBuf_[kRxBufSize]{};
};

} // namespace takt::modules
