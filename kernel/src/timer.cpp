// Copyright 2024-2026 Masyukov Pavel <p.masyukov@gmail.com>
// SPDX-License-Identifier: Apache-2.0
//
// TAKT OS — https://github.com/Developer-RU/Takt-OS

#include "takt/timer.hpp"
#include "takt/timer_manager.hpp"

namespace takt {

Timer::Timer(uint32_t intervalMs, bool repeat)
    : intervalMs_(intervalMs), repeat_(repeat) {}

Timer::~Timer() {
    TimerManager::instance().unregisterTimer(*this);
}

Timer::Timer(Timer&& other) noexcept
    : handle_(other.handle_)
    , intervalMs_(other.intervalMs_)
    , elapsedMs_(other.elapsedMs_)
    , active_(other.active_)
    , repeat_(other.repeat_)
    , callback_(std::move(other.callback_)) {
    other.handle_ = 0xFFFF;
}

Timer& Timer::operator=(Timer&& other) noexcept {
    if (this != &other) {
        TimerManager::instance().unregisterTimer(*this);
        handle_ = other.handle_;
        intervalMs_ = other.intervalMs_;
        elapsedMs_ = other.elapsedMs_;
        active_ = other.active_;
        repeat_ = other.repeat_;
        callback_ = std::move(other.callback_);
        other.handle_ = 0xFFFF;
    }
    return *this;
}

void Timer::setInterval(uint32_t intervalMs) { intervalMs_ = intervalMs; }
void Timer::setRepeat(bool repeat) { repeat_ = repeat; }

void Timer::onTimeout(TimerCallback callback) {
    callback_ = std::move(callback);
    TimerManager::instance().registerTimer(*this);
}

void Timer::start() {
    elapsedMs_ = 0;
    active_ = true;
}

void Timer::stop() { active_ = false; }
void Timer::reset() { elapsedMs_ = 0; }

void Timer::tick(uint32_t deltaMs) {
    if (!active_) return;
    elapsedMs_ += deltaMs;
    if (elapsedMs_ >= intervalMs_) {
        if (callback_) callback_();
        if (repeat_) {
            elapsedMs_ = 0;
        } else {
            active_ = false;
        }
    }
}

#ifdef TAKT_HOST_BUILD
TimerManager& TimerManager::instance() {
    static TimerManager mgr;
    return mgr;
}
#endif

bool TimerManager::registerTimer(Timer& timer) {
    if (activeCount_ >= kMaxTimers) return false;
    for (size_t i = 0; i < kMaxTimers; ++i) {
        if (timers_[i] == nullptr) {
            timers_[i] = &timer;
            timer.setHandle(static_cast<uint16_t>(i));
            ++activeCount_;
            return true;
        }
    }
    return false;
}

void TimerManager::unregisterTimer(Timer& timer) {
    uint16_t h = timer.handle();
    if (h < kMaxTimers && timers_[h] == &timer) {
        timers_[h] = nullptr;
        timer.setHandle(0xFFFF);
        --activeCount_;
    }
}

void TimerManager::tick(uint32_t deltaMs) {
    for (size_t i = 0; i < kMaxTimers; ++i) {
        if (timers_[i]) timers_[i]->tick(deltaMs);
    }
}

} // namespace takt
