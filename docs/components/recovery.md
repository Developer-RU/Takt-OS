# Recovery (`recovery/`)

ESP-IDF component name: **`recovery`**

Firmware recovery and device firmware update (DFU) outside the main application path. Used by `OtaService` and standalone `recovery_app`.

## Components

| File | Role |
|------|------|
| `recovery_manager.cpp` | Orchestrates DFU session, progress, finalize, rollback |
| `recovery_transports.cpp` | BLE, WiFi, UART transport implementations |

## Public API (`include/takt/`)

| Header | Key API |
|--------|---------|
| `recovery_manager.hpp` | `RecoveryManager::instance()`, `init()`, `startDfuListener()`, `receiveChunk()`, `finalizeDfu()`, `rollback()`, `abort()` |
| `recovery_transports.hpp` | `BleDfu`, `WiFiOta`, `UartDfu` |

## Channels

```cpp
enum class RecoveryChannel { Ble, WiFi, Uart };
```

## Rollback

Uses `FirmwareCache` CRC validation before activating a slot. On repeated boot failures, `takt_boot` can force recovery partition.

## Standalone firmware

`examples/recovery_app` — minimal firmware flashed to **recovery** partition (`0x110000`, subtype `test`).

## Depends on

`kernel`, `drivers`, ESP-IDF `app_update`, network stacks per transport

## See also

- [recovery.md](../recovery.md)
- [takt_boot.md](takt_boot.md)
