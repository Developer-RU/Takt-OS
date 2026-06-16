// Copyright 2024-2026 Masyukov Pavel <p.masyukov@gmail.com>
// SPDX-License-Identifier: Apache-2.0
//
// TAKT OS — https://github.com/Developer-RU/Takt-OS

#pragma once

#include "takt/scheduler.hpp"
#include "takt/event_bus.hpp"
#include "takt/timer_manager.hpp"
#include "takt/types.hpp"
#include <cstdint>

namespace takt {

/// Global kernel statistics aggregate.
struct KernelStats {
    SchedulerStats  scheduler;
    size_t          heapFree     = 0;
    size_t          heapMinFree  = 0;
    size_t          largestBlock = 0;
    uint32_t        uptimeMs     = 0;
    uint32_t        resetReason  = 0;
};

/// TAKT OS Kernel — top-level system orchestrator.
///
/// Owns the Scheduler, EventBus, and TimerManager.
/// Provides system lifecycle, diagnostics, and memory monitoring.
class Kernel {
public:
    Kernel() = default;

    static Kernel& instance() {
        static Kernel kernel;
        return kernel;
    }

    /// System boot sequence.
    bool boot();

    /// Access the scheduler.
    Scheduler& scheduler() { return scheduler_; }
    const Scheduler& scheduler() const { return scheduler_; }

    /// Access event bus.
    EventBus& eventBus() { return EventBus::instance(); }

    /// Access timer manager.
    TimerManager& timerManager() { return TimerManager::instance(); }

    /// Enter main takt loop. Does not return.
    void run();

    /// Request graceful shutdown.
    void requestShutdown();

    /// Print full diagnostic report to log output.
    void printStatistics() const;

    /// Collect current kernel statistics.
    KernelStats collectStats() const;

    bool isRunning() const { return running_; }

private:
    Scheduler scheduler_;
    bool      running_   = false;
    bool      shutdown_  = false;
    uint32_t  bootTimeMs_ = 0;

    void updateMemoryStats(KernelStats& stats) const;
    void detectStackOverflow();
};

} // namespace takt
