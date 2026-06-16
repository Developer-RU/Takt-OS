<div align="center">

<img src="https://raw.githubusercontent.com/Developer-RU/Takt-OS/main/docs/assets/takt-os-banner.svg" alt="TAKT OS — Industrial cycle-based RTOS for ESP32" width="100%"/>

<br/>

[![Version](https://img.shields.io/badge/version-0.2.0-38bdf8?style=for-the-badge&logo=semantic-release&logoColor=white)](CHANGELOG.md)
[![License](https://img.shields.io/badge/license-Apache%202.0-22c55e?style=for-the-badge&logo=apache&logoColor=white)](LICENSE)
[![ESP-IDF](https://img.shields.io/badge/ESP--IDF-5.0%2B-e7352c?style=for-the-badge&logo=espressif&logoColor=white)](https://github.com/espressif/esp-idf)
[![Platform](https://img.shields.io/badge/chip-ESP32-ff6b00?style=for-the-badge&logo=espressif&logoColor=white)](https://www.espressif.com/en/products/socs/esp32)

[![C++](https://img.shields.io/badge/C%2B%2B-17-00599c?style=for-the-badge&logo=cplusplus&logoColor=white)](https://isocpp.org/)
[![CMake](https://img.shields.io/badge/CMake-3.16%2B-064f8c?style=for-the-badge&logo=cmake&logoColor=white)](https://cmake.org/)
[![PlatformIO](https://img.shields.io/badge/PlatformIO-ready-f5822a?style=for-the-badge&logo=platformio&logoColor=white)](docs/platformio_vscode.md)
[![FreeRTOS](https://img.shields.io/badge/runtime-FreeRTOS-7e57c2?style=for-the-badge)](https://www.freertos.org/)

[![Docs](https://img.shields.io/badge/documentation-full-64748b?style=for-the-badge&logo=readthedocs&logoColor=white)](docs/README.md)
[![Wiki](https://img.shields.io/badge/wiki-navigation-181717?style=for-the-badge&logo=github&logoColor=white)](docs/wiki/Home.md)
[![CI](https://img.shields.io/github/actions/workflow/status/Developer-RU/Takt-OS/ci.yml?branch=main&style=for-the-badge&label=CI&logo=githubactions&logoColor=white)](https://github.com/Developer-RU/Takt-OS/actions)
[![PRs Welcome](https://img.shields.io/badge/PRs-welcome-8b5cf6?style=for-the-badge&logo=github&logoColor=white)](CONTRIBUTING.md)

**Deterministic takt scheduling for industrial ESP32 firmware**

[Docs](docs/README.md) · [Changelog](CHANGELOG.md) · [Issues](https://github.com/Developer-RU/Takt-OS/issues)

</div>

---

## Overview

**TAKT OS** is an open-source industrial real-time **framework** for Espressif ESP32. It layers a **PLC-style scan cycle** on top of ESP-IDF and FreeRTOS: every **takt** (tick), the kernel walks all registered modules in a **fixed order** — predictable, measurable, and debuggable.

> **Takt** *(German: beat, cycle)* — one kernel cycle where every module gets its turn, like a PLC scan list.

| | FreeRTOS (default app model) | TAKT OS |
|---|------------------------------|---------|
| Unit of work | Preemptive tasks + priorities | `IModule` in fixed order |
| Timing model | Best-effort, priority inversion risk | Deterministic sequence per takt |
| Best for | General multitasking | Modbus, sensors, relays, telemetry, PLC control |
| Platform | ESP-IDF | ESP-IDF + TAKT kernel |

ESP-IDF still owns hardware, WiFi/BLE, flash, OTA bootloader, and system tasks. TAKT OS owns **your application logic**.

---

## Key features

<table>
<tr>
<td width="50%">

### Kernel
- **Takt scheduler** — Static / Dynamic / Background modules
- **EventBus** — sync + queued pub/sub
- **TimerManager** — software timers on takt period
- **Overrun detection** — budget per takt in µs
- **Diagnostics** — per-module stats

</td>
<td width="50%">

### Platform & industrial stack
- **Drivers** — GPIO, UART, ADC, flash HAL
- **Middleware** — Modbus RTU, WiFi, MQTT, HTTP, BLE stub
- **Services** — OTA, telemetry batching, remote config
- **Recovery** — BLE / WiFi / UART DFU + rollback
- **SDK** — C API for third-party modules

</td>
</tr>
</table>

**Shipped examples:** `demo_controller` (reference demo) · `recovery_app` (standalone DFU)  
**Variants:** TAKT OS Lite · multi-target configs (ESP32-S3, ESP32-C3)

---

## Architecture

```
┌─────────────────────────────────────────────────────────────┐
│  Application     demo_controller · your firmware              │
├─────────────────────────────────────────────────────────────┤
│  Middleware      UART · Sensor · Modbus · WiFi · MQTT · Web │
├─────────────────────────────────────────────────────────────┤
│  Services        OTA · Telemetry · ConfigManager · Gateway  │
├─────────────────────────────────────────────────────────────┤
│  Kernel          Scheduler · EventBus · Timers · Storage    │
├─────────────────────────────────────────────────────────────┤
│  Drivers         GPIO · UART · ADC · Platform HAL           │
├─────────────────────────────────────────────────────────────┤
│  takt_boot       Boot policy · recovery jump · OTA select   │
├─────────────────────────────────────────────────────────────┤
│  ESP-IDF         WiFi/BLE · NVS · OTA bootloader · FreeRTOS │
├─────────────────────────────────────────────────────────────┤
│  ESP32           Hardware                                   │
└─────────────────────────────────────────────────────────────┘
```

Each **takt** (default 1 ms):

```
Timer tick → dispatch events → UART → Sensor → Relay → Modbus → WiFi → MQTT → … → stats
```

Deep dive: [docs/architecture.md](docs/architecture.md) · [UML diagrams](docs/uml/diagrams.md)

---

## Module types

| Type | `hasWork()` | Budget | Use case |
|------|-------------|--------|----------|
| **Static** | always | `budgetUs()` enforced | UART read, ADC sample, GPIO |
| **Dynamic** | always | self-paced | Script engine, parsers |
| **Background** | when busy | skipped if idle | WiFi, MQTT, OTA, WebServer |

Minimal module:

```cpp
class MyModule : public takt::IModule {
public:
    bool init() override { return true; }
    void tick() override { /* bounded work */ }
    void shutdown() override {}
    const char* name() const override { return "MyMod"; }
    takt::ModuleType type() const override { return takt::ModuleType::Static; }
    uint32_t budgetUs() const override { return 500; }
};
```

Guide: [docs/developer_guide.md](docs/developer_guide.md)

---

## Quick start

### Requirements

| Tool | Version |
|------|---------|
| [ESP-IDF](https://docs.espressif.com/projects/esp-idf/) or [PlatformIO](https://platformio.org/) | IDF **≥ 5.0** |
| CMake | **≥ 3.16** |
| Python | **≥ 3.8** |
| Hardware | ESP32 (tested: ESP32-D0WD-V3) |

> Clone path must **not contain spaces** (PlatformIO + ESP-IDF constraint).  
> Do **not** commit `build/`, `.pio/`, or `sdkconfig` — they are machine-specific.

### ESP-IDF

```bash
git clone https://github.com/Developer-RU/Takt-OS.git
cd Takt-OS/examples/demo_controller

source $IDF_PATH/export.sh
idf.py set-target esp32      # once after clone
idf.py build
idf.py -p PORT flash monitor # PORT = /dev/ttyUSB0, COM3, …
```

### PlatformIO / VS Code

1. Install [**PlatformIO IDE**](https://platformio.org/install/ide?install=vscode)
2. Open [`Takt-OS.code-workspace`](Takt-OS.code-workspace)
3. Sidebar: **Build** → **Upload** → **Monitor**

Details: [docs/platformio_vscode.md](docs/platformio_vscode.md)

### Host unit tests

```bash
cd tests
cmake -B build && cmake --build build
ctest --test-dir build --output-on-failure
```

---

## Project layout

```
Takt-OS/
├── kernel/           Scheduler, EventBus, timers, NVS, firmware cache
├── drivers/          GPIO, UART, ADC, platform HAL
├── middleware/       UART, WiFi, MQTT, Modbus, WebServer, …
├── services/         OTA, telemetry, config, Modbus gateway
├── takt_boot/        Boot mode policy (not ESP-IDF bootloader)
├── recovery/         DFU transports, rollback
├── examples/
│   ├── demo_controller/    Main demo firmware
│   └── recovery_app/       Recovery partition app
├── sdk/              C API (takt_sdk.h)
├── lite/             Reduced-footprint kernel
├── targets/          esp32s3 / esp32c3 sdkconfig.defaults
├── docs/             Architecture, API, wiki, assets
├── tools/            partitions.csv, test scripts
└── tests/            Host GTest suite
```

Component reference: [docs/components/](docs/components/) · File map: [docs/source_tree.md](docs/source_tree.md)

---

## Documentation

| | |
|---|---|
| [docs/README.md](docs/README.md) | Full documentation (primary source) |
| [docs/wiki/Home.md](docs/wiki/Home.md) | Wiki: navigation index |
| [docs/faq.md](docs/faq.md) | Frequently asked questions |
| [CHANGELOG.md](CHANGELOG.md) | Version history |

---

## Flash partitions

From [`tools/partitions.csv`](tools/partitions.csv) (also in `examples/demo_controller/`):

| Label | Offset | Size | Role |
|-------|--------|------|------|
| `factory` | 0x10000 | 1 MB | Main app (`demo_controller`) |
| `recovery` | 0x110000 | 256 KB | Recovery app (subtype `test`) |
| `ota_0` / `ota_1` | … | 1.25 MB | OTA slots |
| `storage` | 0x3D0000 | 192 KB | Raw data region |

---

## Author

**Masyukov Pavel** — [p.masyukov@gmail.com](mailto:p.masyukov@gmail.com)  
Developer and maintainer of TAKT OS.

---

## Contributing & security

- **[CONTRIBUTING.md](CONTRIBUTING.md)** — setup, code style, PR checklist  
- **[SECURITY.md](SECURITY.md)** — responsible disclosure  
- **[CODE_OF_CONDUCT.md](CODE_OF_CONDUCT.md)** — community standards  

---

## License

**Apache License 2.0** — see [LICENSE](LICENSE).

You may copy, modify, and redistribute **with attribution**:

> Based on [TAKT OS](https://github.com/Developer-RU/Takt-OS) by Masyukov Pavel, Copyright 2024-2026 Masyukov Pavel, [Apache License 2.0](LICENSE).

Third-party notices (ESP-IDF, FreeRTOS, lwIP, …): [NOTICE](NOTICE) · [docs/licensing.md](docs/licensing.md)

---

<div align="center">

**[⭐ Star us on GitHub](https://github.com/Developer-RU/Takt-OS)** if TAKT OS is useful for your project.

Made for industrial ESP32 firmware · **TAKT OS v0.2.0** · Masyukov Pavel

</div>
