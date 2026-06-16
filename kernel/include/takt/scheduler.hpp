// Copyright 2024-2026 Masyukov Pavel <p.masyukov@gmail.com>
// SPDX-License-Identifier: Apache-2.0
//
// TAKT OS — https://github.com/Developer-RU/Takt-OS

#pragma once

#include "takt/imodule.hpp"
#include "takt/types.hpp"
#include <cstddef>
#include <cstdint>

namespace takt {

static constexpr size_t kMaxModules = 48;

/// Per-takt scheduler statistics.
struct SchedulerStats {
    uint64_t totalTakts        = 0;
    uint64_t totalTaktUs       = 0;
    uint64_t maxTaktUs         = 0;
    uint64_t lastTaktUs        = 0;
    uint32_t overrunCount      = 0;
    uint32_t registeredModules = 0;
};

/// TAKT OS Scheduler — the heart of the takt-based execution model.
///
/// Each call to runTakt() constitutes one takt cycle:
///   1. Advance timers
///   2. Dispatch queued events
///   3. Sequentially call tick() on every registered module
///   4. Collect per-module timing statistics
///   5. Detect takt overrun
class Scheduler {
public:
    Scheduler() = default;

    /// Register a module. Modules are called in registration order.
    /// @return ModuleId on success, kInvalidModuleId on failure.
    ModuleId registerModule(IModule* module);

    /// Remove a module from the takt cycle.
    void unregisterModule(ModuleId id);

    /// Initialize all registered modules (calls init()).
    /// @return false if any module fails init.
    bool initAll();

    /// Execute one complete takt cycle.
    void runTakt();

    /// Run the infinite takt loop. Blocks forever.
    void run();

    /// Graceful shutdown — calls shutdown() on all modules.
    void shutdownAll();

    /// Access per-module statistics.
    const ModuleStats& moduleStats(ModuleId id) const;

    /// Access global scheduler statistics.
    const SchedulerStats& stats() const { return stats_; }

    /// Set maximum allowed takt duration in microseconds (0 = no limit).
    void setTaktBudgetUs(uint32_t budgetUs) { taktBudgetUs_ = budgetUs; }

    /// Set takt period in milliseconds (for timer resolution).
    void setTaktPeriodMs(uint32_t periodMs) { taktPeriodMs_ = periodMs; }

    size_t moduleCount() const { return moduleCount_; }

private:
    struct ModuleEntry {
        IModule*    module = nullptr;
        ModuleStats stats{};
        bool        active = false;
    };

    ModuleEntry    modules_[kMaxModules]{};
    size_t         moduleCount_ = 0;
    SchedulerStats stats_{};
    uint32_t       taktBudgetUs_ = 0;
    uint32_t       taktPeriodMs_ = 1; // default 1 ms takt

    bool shouldRunModule(const ModuleEntry& entry) const;
    uint64_t nowUs() const;
};

} // namespace takt
