// Copyright 2024-2026 Masyukov Pavel <p.masyukov@gmail.com>
// SPDX-License-Identifier: Apache-2.0
//
// TAKT OS — https://github.com/Developer-RU/Takt-OS

#pragma once

#include "takt/imodule.hpp"
#include <cstddef>

namespace takt::modules {

/// Dynamic module: executes instructions until work queue is empty.
class ScriptEngineModule : public IModule {
public:
    bool init() override;
    void tick() override;
    void shutdown() override;
    const char* name() const override { return "ScriptEngine"; }
    ModuleType type() const override { return ModuleType::Dynamic; }

    bool hasWork() const override { return workPending_; }
    void enqueueInstruction(uint32_t opcode, uint32_t operand);

private:
    struct Instruction {
        uint32_t opcode;
        uint32_t operand;
    };

    static constexpr size_t kQueueSize = 64;
    Instruction queue_[kQueueSize]{};
    size_t queueHead_ = 0;
    size_t queueTail_ = 0;
    size_t queueCount_ = 0;
    bool workPending_ = false;

    bool executeNextInstruction();
};

} // namespace takt::modules
