#include <gtest/gtest.h>
#include "takt/event_bus.hpp"

namespace {

bool g_received = false;

void onEvent(const takt::EventData& data, void* /*ctx*/) {
    if (data.id == takt::Event::WiFiConnected) g_received = true;
}

TEST(EventBusTest, PublishSubscribe) {
    g_received = false;
    auto& bus = takt::EventBus::instance();
    int handle = bus.subscribe(takt::Event::WiFiConnected, onEvent);
    EXPECT_GE(handle, 0);
    bus.publish(takt::Event::WiFiConnected);
    EXPECT_TRUE(g_received);
    bus.unsubscribe(handle);
}

TEST(EventBusTest, EnqueueAndDispatch) {
    g_received = false;
    auto& bus = takt::EventBus::instance();
    bus.subscribe(takt::Event::WiFiConnected, onEvent);
    takt::EventData data{};
    data.id = takt::Event::WiFiConnected;
    EXPECT_TRUE(bus.enqueue(data));
    bus.dispatchQueued();
    EXPECT_TRUE(g_received);
}

} // namespace
