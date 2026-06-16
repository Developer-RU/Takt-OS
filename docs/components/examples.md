# Examples (`examples/`)

Demonstration firmware and integration reference.

## demo_controller

**Path:** `examples/demo_controller/`  
**Partition:** `factory` @ `0x10000`  
**Purpose:** Reference demo — industrial controller

### Registered modules

UART, Sensor, Relay (GPIO26), Modbus RTU, WiFi, MQTT, BLE, WebServer, OTA, Telemetry, ConfigManager, ModbusGateway.

### Build

```bash
cd examples/demo_controller
idf.py set-target esp32
idf.py build flash monitor
```

PlatformIO: open folder or use root workspace — see [platformio_vscode.md](../platformio_vscode.md).

### Config files

| File | Purpose |
|------|---------|
| `sdkconfig.defaults` | FreeRTOS 1 kHz, 4 MB flash, 12 KB main stack |
| `partitions.csv` | Factory, recovery, OTA slots, storage |
| `platformio.ini` | PlatformIO env `esp32` |

### HTTP API

- `GET /api/status` — system status JSON
- `GET /api/stats` — scheduler statistics

---

## recovery_app

**Path:** `examples/recovery_app/`  
**Partition:** `recovery` @ `0x110000` (subtype `test`)  
**Purpose:** Standalone DFU / recovery firmware

### Build

```bash
cd examples/recovery_app
idf.py set-target esp32
idf.py build
# Flash to recovery offset (see platformio.ini board_build.offset)
```

---

## Adding your application

1. Copy `demo_controller/` as template.
2. Update `CMakeLists.txt` `EXTRA_COMPONENT_DIRS` paths to repo root.
3. Register only modules you need.
4. Adjust `partitions.csv` if binary size requires it.

## See also

- [developer_guide.md](../developer_guide.md)
- [components/middleware.md](middleware.md)

---

**TAKT OS** — Developer: **Masyukov Pavel** ([p.masyukov@gmail.com](mailto:p.masyukov@gmail.com)) · License: [Apache License 2.0](https://github.com/Developer-RU/Takt-OS/blob/main/LICENSE) · [Source](https://github.com/Developer-RU/Takt-OS)
