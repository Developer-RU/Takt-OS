#pragma once

#include <cstdint>
#include <functional>

namespace takt::drivers {

/// GPIO abstraction.
class Gpio {
public:
    enum class Mode { Input, Output, InputPullUp, InputPullDown };
    enum class Level { Low = 0, High = 1 };

    static bool setMode(int pin, Mode mode);
    static Level read(int pin);
    static void write(int pin, Level level);
};

/// Hardware UART abstraction.
class Uart {
public:
    struct Config {
        int      port = 0;
        uint32_t baudRate = 115200;
        int      txPin = -1;
        int      rxPin = -1;
    };

    static bool init(const Config& config);
    static int read(int port, uint8_t* buf, size_t len);
    static int write(int port, const uint8_t* buf, size_t len);
};

/// ADC abstraction for sensor reading.
class Adc {
public:
    static bool init(int channel);
    static int readRaw(int channel);
    static float readVoltage(int channel);
};

/// Platform initialization — clocks, watchdog, heap.
class Platform {
public:
    static bool init();
    static void feedWatchdog();
    static uint32_t getResetReason();
    static uint64_t getUptimeUs();
};

} // namespace takt::drivers
