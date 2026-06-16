// Copyright 2024-2026 Masyukov Pavel <p.masyukov@gmail.com>
// SPDX-License-Identifier: Apache-2.0
//
// TAKT OS — https://github.com/Developer-RU/Takt-OS

#pragma once

#include "takt/event.hpp"
#include <cstddef>
#include <cstdint>

namespace takt {

static constexpr size_t kMaxEventSubscribers = 32;
static constexpr size_t kEventQueueDepth     = 64;

/// Publish-subscribe event bus for inter-module communication.
///
/// Thread-safe for single-core ESP32 when called from the takt loop.
/// Deferred delivery: events published during tick() are delivered
/// at the end of the current takt or the start of the next.
class EventBus {
public:
    EventBus() = default;

  #ifdef TAKT_HOST_BUILD
    static EventBus& instance();
  #else
    static EventBus& instance() {
        static EventBus bus;
        return bus;
    }
  #endif

    /// Subscribe to an event. Returns subscription handle or -1 on failure.
    int subscribe(Event event, EventCallback callback, void* userData = nullptr);

    /// Unsubscribe by handle.
    void unsubscribe(int handle);

    /// Publish event immediately (synchronous dispatch to subscribers).
    void publish(const EventData& data);

    /// Convenience overload.
    void publish(Event event, uint32_t param1 = 0, uint32_t param2 = 0);

    /// Queue event for deferred delivery at end of takt.
    bool enqueue(const EventData& data);

    /// Process all queued events. Called by Scheduler at takt boundary.
    void dispatchQueued();

    /// Number of active subscriptions.
    size_t subscriberCount() const { return subscriberCount_; }

private:
    struct Subscription {
        Event         event    = Event::None;
        EventCallback callback = nullptr;
        void*         userData = nullptr;
        bool          active   = false;
    };

    Subscription subscriptions_[kMaxEventSubscribers]{};
    size_t       subscriberCount_ = 0;

    EventData    queue_[kEventQueueDepth]{};
    size_t       queueHead_ = 0;
    size_t       queueTail_ = 0;
    size_t       queueCount_ = 0;

    void dispatch(const EventData& data);
};

} // namespace takt

// Convenience macros
#define TAKT_PUBLISH(ev) \
    takt::EventBus::instance().publish(ev)

#define TAKT_SUBSCRIBE(ev, cb, ctx) \
    takt::EventBus::instance().subscribe(ev, cb, ctx)
