#pragma once

#include "takt/types.hpp"
#include <cstdint>

namespace takt {

/// System-wide event identifiers.
/// Extend this enum for application-specific events (start at 0x1000).
enum class Event : uint32_t {
    None = 0,

    // System events (0x0001 – 0x00FF)
    SystemBoot          = 0x0001,
    SystemReady         = 0x0002,
    SystemShutdown      = 0x0003,
    TaktOverrun         = 0x0004,
    MemoryLow           = 0x0005,
    StackOverflow       = 0x0006,

    // Connectivity (0x0100 – 0x01FF)
    WiFiConnected       = 0x0100,
    WiFiDisconnected    = 0x0101,
    MqttConnected       = 0x0102,
    MqttDisconnected    = 0x0103,
    BleConnected        = 0x0104,
    BleDisconnected     = 0x0105,

    // OTA / Recovery (0x0200 – 0x02FF)
    OtaStart            = 0x0200,
    OtaProgress         = 0x0201,
    OtaComplete         = 0x0202,
    OtaFailed           = 0x0203,
    OtaRollback         = 0x0204,
    RecoveryEnter       = 0x0205,
    RecoveryExit        = 0x0206,

    // Application (0x0300 – 0x03FF)
    SensorDataReady     = 0x0300,
    UartDataReceived    = 0x0301,
    ModbusRequest       = 0x0302,
    CycleStarted        = 0x0303,
    CycleCompleted      = 0x0304,

    // User-defined events start here
    UserDefined         = 0x1000,
};

/// Lightweight event payload (up to 16 bytes inline).
struct EventData {
    Event    id     = Event::None;
    uint32_t param1 = 0;
    uint32_t param2 = 0;
    uint8_t  blob[8]{};
};

using EventCallback = void(*)(const EventData& data, void* userData);

} // namespace takt
