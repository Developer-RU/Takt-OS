// Copyright 2024-2026 Masyukov Pavel <p.masyukov@gmail.com>
// SPDX-License-Identifier: Apache-2.0
//
// TAKT OS — https://github.com/Developer-RU/Takt-OS

#pragma once

#include "takt/timer.hpp"
#include <cstddef>
#include <cstdint>

namespace takt {

static constexpr size_t kMaxTimers = 32;

/// Central timer registry. Advances all active timers each takt.
class TimerManager {
public:
    TimerManager() = default;

  #ifdef TAKT_HOST_BUILD
    static TimerManager& instance();
  #else
    static TimerManager& instance() {
        static TimerManager mgr;
        return mgr;
    }
  #endif

    /// Register a timer for management. Returns false if pool exhausted.
    bool registerTimer(Timer& timer);

    /// Deregister a timer.
    void unregisterTimer(Timer& timer);

    /// Advance all timers. Called once per takt by Scheduler.
    void tick(uint32_t deltaMs);

    size_t activeCount() const { return activeCount_; }

private:
    Timer* timers_[kMaxTimers]{};
    size_t activeCount_ = 0;
};

} // namespace takt
