#include "takt/modules/uart_module.hpp"
#include "takt/drivers/platform.hpp"
#include "takt/event_bus.hpp"
#include "takt/logger.hpp"

namespace takt::modules {

namespace {
constexpr const char* TAG = "UART";
}

UartModule::UartModule(int port, size_t bytesPerTick)
    : port_(port), bytesPerTick_(bytesPerTick) {}

bool UartModule::init() {
    drivers::Uart::Config cfg{};
    cfg.port = port_;
    cfg.baudRate = 115200;
    cfg.txPin = 1;
    cfg.rxPin = 3;
    if (!drivers::Uart::init(cfg)) {
        TAKT_LOGW(TAG, "UART%d init failed, running in stub mode", port_);
    }
    TAKT_LOGI(TAG, "UART%d init, %u bytes/takt", port_, static_cast<unsigned>(bytesPerTick_));
    return true;
}

void UartModule::tick() {
    rxCount_ = static_cast<size_t>(drivers::Uart::read(
        port_, rxBuf_, bytesPerTick_));
    if (rxCount_ > 0) {
        EventBus::instance().publish(Event::UartDataReceived,
                                     static_cast<uint32_t>(rxCount_));
    }
}

void UartModule::shutdown() {
    TAKT_LOGI(TAG, "UART%d shutdown", port_);
}

} // namespace takt::modules
