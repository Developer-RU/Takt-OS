# Services (`services/`)

ESP-IDF component name: **`services`**

Higher-level application services, also implemented as `IModule` where they participate in the takt loop.

## Services

| Service | Header | Type | Role |
|---------|--------|------|------|
| OTA | `ota_service.hpp` | Background | Chunked firmware update, progress, rollback |
| Telemetry | `telemetry_service.hpp` | Background | Batch sensor/event records |
| ConfigManager | `config_manager.hpp` | Background | Remote JSON → NVS |
| ModbusGateway | `modbus_gateway.hpp` | Background | Maps Modbus registers to app data |

## OTA pipeline

```
OtaService::tick()
  → delegates to RecoveryManager for DFU transport
  → FirmwareCache::beginWrite / writeChunk / finalizeWrite
  → esp_ota_set_boot_partition on success
  → rollback via RecoveryManager on failure
```

## Telemetry

- Ring buffer via `CacheManager` (pool provided by application)
- `TelemetryRecord`: timestamp, sensor value, flags
- Flushed over MQTT when connected

## ConfigManager

- Parses JSON config payloads
- Writes namespaced NVS keys (`takt` namespace)
- Publishes `ConfigUpdated` event

## ModbusGateway

- Requires pointer to `ModbusModule`
- Exposes holding registers for device status and stats

## Depends on

`kernel`, `middleware`, `recovery`, `drivers`

## See also

- [recovery.md](recovery.md)
- [api_reference.md](../api_reference.md)

---

**TAKT OS** — Developer: **Masyukov Pavel** ([p.masyukov@gmail.com](mailto:p.masyukov@gmail.com)) · License: [Apache License 2.0](https://github.com/Developer-RU/Takt-OS/blob/main/LICENSE) · [Source](https://github.com/Developer-RU/Takt-OS)
