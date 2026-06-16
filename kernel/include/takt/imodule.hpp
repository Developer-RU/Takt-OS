// Copyright 2024-2026 Masyukov Pavel <p.masyukov@gmail.com>
// SPDX-License-Identifier: Apache-2.0
//
// TAKT OS — https://github.com/Developer-RU/Takt-OS

#pragma once

#include "takt/types.hpp"

namespace takt {

/// Base interface for all TAKT OS modules.
///
/// Every module — static, dynamic, or background — implements this interface.
/// The Scheduler calls init() once, then tick() every takt cycle.
class IModule {
public:
    virtual ~IModule() = default;

    /// One-time initialization. Called before the first tick.
    /// @return true on success, false aborts module registration.
    virtual bool init() = 0;

    /// Called once per takt cycle by the Scheduler.
    virtual void tick() = 0;

    /// Graceful shutdown. Called on system stop or module deregistration.
    virtual void shutdown() = 0;

    /// Human-readable module name for diagnostics.
    virtual const char* name() const = 0;

    /// Module category — affects scheduler dispatch policy.
    virtual ModuleType type() const = 0;

    /// Optional budget in microseconds for static modules (0 = unlimited).
    virtual uint32_t budgetUs() const { return 0; }

    /// Returns true if a background module has pending work.
    virtual bool hasWork() const { return true; }
};

} // namespace takt
