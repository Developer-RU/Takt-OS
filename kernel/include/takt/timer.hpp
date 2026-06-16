// Copyright 2024-2026 Masyukov Pavel <p.masyukov@gmail.com>
// SPDX-License-Identifier: Apache-2.0
//
// TAKT OS — https://github.com/Developer-RU/Takt-OS

#pragma once

#include <cstdint>
#include <functional>

namespace takt {

using TimerCallback = std::function<void()>;

/// Software timer managed by TimerManager.
///
/// Timers are driven by the takt loop — resolution equals takt period.
class Timer {
public:
    Timer() = default;
    Timer(uint32_t intervalMs, bool repeat = false);
    ~Timer();

    Timer(const Timer&) = delete;
    Timer& operator=(const Timer&) = delete;
    Timer(Timer&& other) noexcept;
    Timer& operator=(Timer&& other) noexcept;

    /// Set interval in milliseconds.
    void setInterval(uint32_t intervalMs);

    /// Set one-shot or repeating mode.
    void setRepeat(bool repeat);

    /// Register timeout callback.
    void onTimeout(TimerCallback callback);

    /// Start the timer. Resets elapsed counter.
    void start();

    /// Stop the timer without destroying it.
    void stop();

    /// Reset elapsed counter without stopping.
    void reset();

    bool isActive() const { return active_; }
    bool isRepeat() const { return repeat_; }
    uint32_t intervalMs() const { return intervalMs_; }
    uint32_t elapsedMs() const { return elapsedMs_; }

    /// Called by TimerManager each takt. Do not call directly.
    void tick(uint32_t deltaMs);

    /// Internal handle assigned by TimerManager.
    uint16_t handle() const { return handle_; }
    void setHandle(uint16_t h) { handle_ = h; }

private:
    uint16_t       handle_    = 0xFFFF;
    uint32_t       intervalMs_ = 0;
    uint32_t       elapsedMs_  = 0;
    bool           active_     = false;
    bool           repeat_     = false;
    TimerCallback  callback_;
};

} // namespace takt
