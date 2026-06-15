# Drivers (`drivers/`)

ESP-IDF component name: **`drivers`**

Hardware abstraction layer (HAL) for ESP32. Middleware and services use these APIs instead of calling ESP-IDF directly where possible.

## Source layout

| File | Role |
|------|------|
| `src/platform.cpp` | `Platform::init()`, watchdog, uptime, storage partition init |
| `src/gpio.cpp` | `Gpio` — digital I/O |
| `src/uart.cpp` | `Uart` — serial ports |
| `src/adc.cpp` | `Adc` — analog input with calibration |
| `src/flash_backend.cpp` | Partition-backed flash read/write/erase |

## Public headers (`include/takt/drivers/`)

| Header | Types |
|--------|-------|
| `platform.hpp` | `Platform`, `Gpio`, `Uart`, `Adc` |
| `flash_backend.hpp` | `IFlashBackend`, `createPartitionBackend()` |

## Platform API

```cpp
namespace takt::drivers {
  struct Platform {
    static bool init();
    static void feedWatchdog();
    static uint64_t getUptimeUs();
  };
}
```

`init()` typically: NVS, storage partition, running OTA partition log.

## Flash backend

Used by `StorageManager` and `FirmwareCache`:

- `createPartitionBackend(label)` — bind to named partition
- `initStorageFromPartition(label)` — mount `storage` data region

## ESP-IDF notes

- ADC uses **line fitting** calibration on classic ESP32 (not curve fitting).
- UART pins configured per instance in middleware modules.

## Depends on

`kernel` (logging), ESP-IDF `driver`, `esp_adc`, `nvs_flash`, `esp_partition`

## See also

- [memory.md](../memory.md)
- [middleware.md](middleware.md)
