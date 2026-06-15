# Middleware (`middleware/`)

ESP-IDF component name: **`middleware`**

Application-facing modules implementing `takt::IModule`. Each module is registered with the scheduler and executed every takt (or when `hasWork()` for background modules).

## Modules

| Module | Header | Type | Description |
|--------|--------|------|-------------|
| UART | `uart_module.hpp` | Static | UART0/1 read/write per takt |
| Sensor | `sensor_module.hpp` | Static | ADC sampling |
| Relay | `relay_module.hpp` | Static | GPIO relay control |
| Modbus | `modbus_module.hpp` | Background | Modbus RTU master/slave |
| WiFi | `wifi_module.hpp` | Background | STA mode, reconnect |
| MQTT | `mqtt_module.hpp` | Background | Publish/subscribe |
| BLE | `ble_module.hpp` | Background | BLE stub (NimBLE optional) |
| WebServer | `webserver_module.hpp` | Background | HTTP `/api/status`, `/api/stats` |
| ScriptEngine | `script_engine_module.hpp` | Dynamic | User script execution |

## Source files

```
middleware/src/
  uart_module.cpp
  sensor_module.cpp
  relay_module.cpp
  modbus_module.cpp
  wifi_module.cpp
  mqtt_module.cpp
  ble_module.cpp
  webserver_module.cpp
  script_engine_module.cpp
```

## Typical registration (`demo_controller`)

```cpp
takt::modules::UartModule uart(0, 16);
takt::modules::WiFiModule wifi;
scheduler.registerModule(&uart);
scheduler.registerModule(&wifi);
```

## Events published / consumed

| Module | Events |
|--------|--------|
| WiFi | `WiFiConnected`, `WiFiDisconnected` |
| Sensor | `SensorDataReady` |
| MQTT | subscribes after WiFi up |
| Modbus | register read/write via gateway |

## Configuration

Most modules read defaults from NVS via `ConfigManager` or constructor args (UART port, GPIO pin, broker URL).

**Production:** set `wifi_ssid`, `wifi_pass` in NVS; change MQTT broker from demo `broker.hivemq.com`.

## Depends on

`kernel`, `drivers`, ESP-IDF `esp_wifi`, `mqtt`, `esp_http_server`, etc.

## See also

- [developer_guide.md](../developer_guide.md)
- [services.md](services.md)
