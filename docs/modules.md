# TAKT OS Modules Reference

## Middleware Modules

### UartModule (Static)

| Property | Value |
|----------|-------|
| Type | Static |
| Budget | 500 µs |
| File | `middleware/include/takt/modules/uart_module.hpp` |

Читает фиксированное количество байт из UART за такт. Публикует `Event::UartDataReceived`.

```cpp
takt::modules::UartModule uart(0, 16);  // port 0, 16 bytes/takt
```

### SensorModule (Static)

| Property | Value |
|----------|-------|
| Type | Static |
| Budget | 2000 µs |
| File | `middleware/include/takt/modules/sensor_module.hpp` |

Обрабатывает ровно один sample за такт. Публикует `Event::SensorDataReady`.

### ScriptEngineModule (Dynamic)

| Property | Value |
|----------|-------|
| Type | Dynamic |
| File | `middleware/include/takt/modules/script_engine_module.hpp` |

Выполняет инструкции из очереди до опустошения. Подходит для скриптовых движков и парсеров.

```cpp
scriptEngine.enqueueInstruction(OP_SET_RELAY, relayId);
```

### WiFiModule (Background)

| Property | Value |
|----------|-------|
| Type | Background |
| File | `middleware/include/takt/modules/wifi_module.hpp` |

Управление WiFi-соединением. Активен при reconnect или наличии событий. Публикует `Event::WiFiConnected`.

### MqttModule (Background)

| Property | Value |
|----------|-------|
| Type | Background |
| File | `middleware/include/takt/modules/mqtt_module.hpp` |

MQTT publish/subscribe. Активен при наличии сообщений в очереди.

### BleModule (Background)

| Property | Value |
|----------|-------|
| Type | Background |
| File | `middleware/include/takt/modules/ble_module.hpp` |

Обработка BLE GAP/GATT событий. Пропускается при отсутствии событий.

## Services

### OtaService (Background)

| Property | Value |
|----------|-------|
| Type | Background |
| File | `services/include/takt/services/ota_service.hpp` |

WiFi и BLE OTA с поддержкой отката через FirmwareCache.

## Создание кастомного модуля

```cpp
class WashCycleModule : public takt::IModule {
    enum class State { Idle, Washing, Rinsing, Drying };
    State state_ = State::Idle;

public:
    bool init() override { return true; }
    void tick() override {
        switch (state_) {
            case State::Washing: doWashing(); break;
            case State::Rinsing: doRinsing(); break;
            default: break;
        }
    }
    void shutdown() override { state_ = State::Idle; }
    const char* name() const override { return "WashCycle"; }
    takt::ModuleType type() const override { return takt::ModuleType::Static; }
    uint32_t budgetUs() const override { return 3000; }
};
```
