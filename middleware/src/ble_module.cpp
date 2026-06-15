#include "takt/modules/ble_module.hpp"
#include "takt/event_bus.hpp"
#include "takt/logger.hpp"

namespace takt::modules {

namespace {
constexpr const char* TAG = "BLE";
}

bool BleModule::init() {
#ifdef TAKT_ESP32
    // NimBLE stack initialized when CONFIG_BT_NIMBLE_ENABLED=y
    TAKT_LOGI(TAG, "BLE init (enable CONFIG_BT_NIMBLE_ENABLED in sdkconfig)");
#else
    TAKT_LOGI(TAG, "BLE init (host stub)");
#endif
    return true;
}

void BleModule::processEvents() {
    eventPending_ = false;
}

void BleModule::tick() {
    if (!eventPending_) return;
    processEvents();
}

void BleModule::shutdown() {
    TAKT_LOGI(TAG, "BLE shutdown");
}

} // namespace takt::modules
