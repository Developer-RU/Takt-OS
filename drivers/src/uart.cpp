#include "takt/drivers/platform.hpp"
#include "takt/logger.hpp"

#ifdef TAKT_ESP32
#include "driver/uart.h"
#include "driver/gpio.h"
#endif

namespace takt::drivers {

namespace {
constexpr const char* TAG = "Uart";
#ifdef TAKT_ESP32
constexpr int kMaxPorts = 3;
bool installed[kMaxPorts]{};
#endif
}

bool Uart::init(const Config& config) {
#ifdef TAKT_ESP32
    if (config.port < 0 || config.port >= kMaxPorts) return false;
    uart_config_t uartConfig = {
        .baud_rate = static_cast<int>(config.baudRate),
        .data_bits = UART_DATA_8_BITS,
        .parity    = UART_PARITY_DISABLE,
        .stop_bits = UART_STOP_BITS_1,
        .flow_ctrl = UART_HW_FLOWCTRL_DISABLE,
        .rx_flow_ctrl_thresh = 0,
        .source_clk = UART_SCLK_DEFAULT,
    };
    if (uart_param_config(static_cast<uart_port_t>(config.port), &uartConfig) != ESP_OK) {
        return false;
    }
    if (config.txPin >= 0 && config.rxPin >= 0) {
        if (uart_set_pin(static_cast<uart_port_t>(config.port),
                         config.txPin, config.rxPin,
                         UART_PIN_NO_CHANGE, UART_PIN_NO_CHANGE) != ESP_OK) {
            return false;
        }
    }
    if (uart_driver_install(static_cast<uart_port_t>(config.port), 2048, 0, 0, nullptr, 0) != ESP_OK) {
        return false;
    }
    installed[config.port] = true;
    TAKT_LOGI(TAG, "UART%d init @ %u baud", config.port, config.baudRate);
    return true;
#else
    (void)config;
    return true;
#endif
}

int Uart::read(int port, uint8_t* buf, size_t len) {
#ifdef TAKT_ESP32
    if (port < 0 || port >= kMaxPorts || !installed[port]) return -1;
    const int n = uart_read_bytes(static_cast<uart_port_t>(port), buf, len, 0);
    return n < 0 ? -1 : n;
#else
    (void)port; (void)buf; (void)len;
    return 0;
#endif
}

int Uart::write(int port, const uint8_t* buf, size_t len) {
#ifdef TAKT_ESP32
    if (port < 0 || port >= kMaxPorts || !installed[port]) return -1;
    const int n = uart_write_bytes(static_cast<uart_port_t>(port),
                                   reinterpret_cast<const char*>(buf), len);
    return n < 0 ? -1 : n;
#else
    (void)port; (void)buf; (void)len;
    return static_cast<int>(len);
#endif
}

} // namespace takt::drivers
