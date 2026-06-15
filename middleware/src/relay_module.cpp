#include "takt/modules/relay_module.hpp"
#include "takt/event_bus.hpp"
#include "takt/logger.hpp"

namespace takt::modules {

namespace {
constexpr const char* TAG = "Relay";
}

RelayModule::RelayModule(int gpioPin) : gpioPin_(gpioPin) {}

bool RelayModule::init() {
    drivers::Gpio::setMode(gpioPin_, drivers::Gpio::Mode::Output);
    drivers::Gpio::write(gpioPin_, drivers::Gpio::Level::Low);
    TAKT_LOGI(TAG, "Relay on GPIO%d", gpioPin_);
    return true;
}

void RelayModule::setRelay(bool on) {
    targetState_ = on;
    pending_ = true;
}

void RelayModule::tick() {
    if (!pending_) return;
    pending_ = false;
    relayOn_ = targetState_;
    drivers::Gpio::write(gpioPin_,
        relayOn_ ? drivers::Gpio::Level::High : drivers::Gpio::Level::Low);
    EventBus::instance().publish(
        relayOn_ ? Event::WashCycleStart : Event::WashCycleComplete);
    TAKT_LOGI(TAG, "Relay %s", relayOn_ ? "ON" : "OFF");
}

void RelayModule::shutdown() {
    setRelay(false);
    tick();
    TAKT_LOGI(TAG, "Relay shutdown");
}

} // namespace takt::modules
