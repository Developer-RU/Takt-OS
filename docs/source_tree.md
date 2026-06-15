# Source tree reference

Complete map of source and header files (excluding `build/`, `.pio/`, `.git/`).

## Root

| File | Purpose |
|------|---------|
| `CMakeLists.txt` | Root CMake project, version, subdirs |
| `README.md` | Project overview |
| `LICENSE` | Apache 2.0 |
| `NOTICE` | Attributions |
| `CHANGELOG.md` | Release history |
| `CONTRIBUTING.md` | Contribution guide |
| `Takt-OS.code-workspace` | VS Code multi-root workspace |

## kernel/

```
include/takt/
  kernel.hpp, scheduler.hpp, imodule.hpp, types.hpp
  event.hpp, event_bus.hpp
  timer.hpp, timer_manager.hpp
  logger.hpp, diagnostics.hpp
  storage_manager.hpp, nvs_manager.hpp, firmware_cache.hpp, cache_manager.hpp
  crc32.hpp
src/
  kernel.cpp, scheduler.cpp, event_bus.cpp, timer_manager.cpp
  storage_manager.cpp, nvs_manager.cpp, firmware_cache.cpp, cache_manager.cpp
  diagnostics.cpp, crc32.cpp
```

## drivers/

```
include/takt/drivers/
  platform.hpp, flash_backend.hpp
src/
  platform.cpp, gpio.cpp, uart.cpp, adc.cpp, flash_backend.cpp
```

## middleware/

```
include/takt/modules/
  uart_module.hpp, sensor_module.hpp, relay_module.hpp
  modbus_module.hpp, wifi_module.hpp, mqtt_module.hpp
  ble_module.hpp, webserver_module.hpp, script_engine_module.hpp
src/
  (matching .cpp for each module)
```

## services/

```
include/takt/services/
  ota_service.hpp, telemetry_service.hpp
  config_manager.hpp, modbus_gateway.hpp
src/
  ota_service.cpp, telemetry_service.cpp
  config_manager.cpp, modbus_gateway.cpp
```

## takt_boot/

```
include/takt/bootloader.hpp
src/bootloader.cpp
```

## recovery/

```
include/takt/
  recovery_manager.hpp, recovery_transports.hpp
src/
  recovery_manager.cpp, recovery_transports.cpp
```

## examples/

```
demo_controller/
  CMakeLists.txt, platformio.ini, sdkconfig.defaults, partitions.csv
  main/main.cpp, main/CMakeLists.txt
recovery_app/
  CMakeLists.txt, platformio.ini
  main/main.cpp, main/CMakeLists.txt
```

## sdk/

```
include/takt_sdk.h
```

## lite/

```
kernel_lite.cpp, CMakeLists.txt
```

## targets/

```
esp32s3/sdkconfig.defaults
esp32c3/sdkconfig.defaults
```

## tools/

```
partitions.csv
stress_test.py
manufacturing_test.py
```

## tests/

```
CMakeLists.txt
test_scheduler.cpp, test_event_bus.cpp, test_timer.cpp
test_nvs_manager.cpp, test_firmware_cache.cpp, test_bootloader.cpp
```

## docs/

See [docs/README.md](README.md) for full documentation index.

## ESP-IDF component names

When using `REQUIRES` in `CMakeLists.txt`, use **directory names**:

`kernel`, `drivers`, `middleware`, `services`, `recovery`, `takt_boot`
