# TAKT OS Modules Reference

## Middleware Modules

### UartModule (Static)

| Property | Value |
|----------|-------|
| Type | Static |
| Budget | 500 µs |
| File | `middleware/include/takt/modules/uart_module.hpp` |

Reads a fixed number of bytes from UART per takt cycle. Publishes `Event::UartDataReceived`.

```cpp
takt::modules::UartModule uart(0, 16);  // port 0, 16 bytes/takt
```

### SensorModule (Static)

| Property | Value |
|----------|-------|
| Type | Static |
| Budget | 2000 µs |
| File | `middleware/include/takt/modules/sensor_module.hpp` |

Processes exactly one sample per takt cycle. Publishes `Event::SensorDataReady`.

### ScriptEngineModule (Dynamic)

| Property | Value |
|----------|-------|
| Type | Dynamic |
| File | `middleware/include/takt/modules/script_engine_module.hpp` |

Executes instructions from a queue until empty. Suitable for script engines and parsers.

```cpp
scriptEngine.enqueueInstruction(OP_SET_RELAY, relayId);
```

### WiFiModule (Background)

| Property | Value |
|----------|-------|
| Type | Background |
| File | `middleware/include/takt/modules/wifi_module.hpp` |

Manages WiFi connection. Active during reconnect or when events are pending. Publishes `Event::WiFiConnected`.

### MqttModule (Background)

| Property | Value |
|----------|-------|
| Type | Background |
| File | `middleware/include/takt/modules/mqtt_module.hpp` |

MQTT publish/subscribe. Active when messages are queued.

### BleModule (Background)

| Property | Value |
|----------|-------|
| Type | Background |
| File | `middleware/include/takt/modules/ble_module.hpp` |

Handles BLE GAP/GATT events. Skipped when no events are pending.

## Services

### OtaService (Background)

| Property | Value |
|----------|-------|
| Type | Background |
| File | `services/include/takt/services/ota_service.hpp` |

WiFi and BLE OTA with rollback support via FirmwareCache.

## Creating a Custom Module

```cpp
class ProcessCycleModule : public takt::IModule {
    enum class State { Idle, Running, Cooling, Done };
    State state_ = State::Idle;

public:
    bool init() override { return true; }
    void tick() override {
        switch (state_) {
            case State::Running: doRunning(); break;
            case State::Cooling: doCooling(); break;
            default: break;
        }
    }
    void shutdown() override { state_ = State::Idle; }
    const char* name() const override { return "ProcessCycle"; }
    takt::ModuleType type() const override { return takt::ModuleType::Static; }
    uint32_t budgetUs() const override { return 3000; }
};
```

---

**TAKT OS** — Developer: **Masyukov Pavel** ([p.masyukov@gmail.com](mailto:p.masyukov@gmail.com)) · License: [Apache License 2.0](https://github.com/Developer-RU/Takt-OS/blob/main/LICENSE) · [Source](https://github.com/Developer-RU/Takt-OS)
