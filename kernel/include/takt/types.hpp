// Copyright 2024-2026 Masyukov Pavel <p.masyukov@gmail.com>
// SPDX-License-Identifier: Apache-2.0
//
// TAKT OS — https://github.com/Developer-RU/Takt-OS

#pragma once

#include <cstdint>
#include <cstddef>

namespace takt {

/// Module execution category — determines scheduler behavior per takt.
enum class ModuleType : uint8_t {
    Static,      ///< Fixed work per takt
    Dynamic,     ///< Self-paced work per takt
    Background,  ///< Event-driven, skipped when idle
};

/// Module lifecycle and runtime state.
enum class ModuleState : uint8_t {
    Unregistered,
    Registered,
    Initializing,
    Ready,
    Running,
    Suspended,
    Error,
    Shutdown,
};

/// Unique module identifier assigned at registration.
using ModuleId = uint16_t;
static constexpr ModuleId kInvalidModuleId = 0xFFFF;

/// Per-module runtime statistics collected by the Scheduler.
struct ModuleStats {
    uint64_t tickCount       = 0;
    uint64_t totalUs         = 0;
    uint64_t maxUs           = 0;
    uint64_t lastUs          = 0;
    uint32_t overrunCount    = 0;
    uint32_t skipCount       = 0;
    ModuleState state        = ModuleState::Unregistered;
};

} // namespace takt
