// Copyright 2024-2026 Masyukov Pavel <p.masyukov@gmail.com>
// SPDX-License-Identifier: Apache-2.0
//
// TAKT OS — https://github.com/Developer-RU/Takt-OS

#include "takt/event_bus.hpp"
#include "takt/logger.hpp"

namespace takt {

#ifdef TAKT_HOST_BUILD
EventBus& EventBus::instance() {
    static EventBus bus;
    return bus;
}
#endif

int EventBus::subscribe(Event event, EventCallback callback, void* userData) {
    if (subscriberCount_ >= kMaxEventSubscribers) {
        TAKT_LOGE("EventBus", "Subscriber pool exhausted");
        return -1;
    }
    for (size_t i = 0; i < kMaxEventSubscribers; ++i) {
        if (!subscriptions_[i].active) {
            subscriptions_[i] = {event, callback, userData, true};
            ++subscriberCount_;
            return static_cast<int>(i);
        }
    }
    return -1;
}

void EventBus::unsubscribe(int handle) {
    if (handle < 0 || handle >= static_cast<int>(kMaxEventSubscribers)) return;
    if (subscriptions_[handle].active) {
        subscriptions_[handle].active = false;
        --subscriberCount_;
    }
}

void EventBus::publish(const EventData& data) {
    dispatch(data);
}

void EventBus::publish(Event event, uint32_t param1, uint32_t param2) {
    EventData data{};
    data.id = event;
    data.param1 = param1;
    data.param2 = param2;
    dispatch(data);
}

bool EventBus::enqueue(const EventData& data) {
    if (queueCount_ >= kEventQueueDepth) {
        TAKT_LOGW("EventBus", "Event queue full, dropping event %u", static_cast<uint32_t>(data.id));
        return false;
    }
    queue_[queueTail_] = data;
    queueTail_ = (queueTail_ + 1) % kEventQueueDepth;
    ++queueCount_;
    return true;
}

void EventBus::dispatchQueued() {
    while (queueCount_ > 0) {
        EventData data = queue_[queueHead_];
        queueHead_ = (queueHead_ + 1) % kEventQueueDepth;
        --queueCount_;
        dispatch(data);
    }
}

void EventBus::dispatch(const EventData& data) {
    for (size_t i = 0; i < kMaxEventSubscribers; ++i) {
        if (subscriptions_[i].active &&
            subscriptions_[i].event == data.id &&
            subscriptions_[i].callback) {
            subscriptions_[i].callback(data, subscriptions_[i].userData);
        }
    }
}

} // namespace takt
