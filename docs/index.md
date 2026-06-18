---
layout: default
title: Home
---

<img class="banner" src="{{ '/assets/takt-os-banner.svg' | relative_url }}" alt="TAKT OS — Industrial cycle-based RTOS for ESP32">

<p class="hero-tagline"><strong>Deterministic takt scheduling for industrial ESP32 firmware.</strong></p>

<p class="hero-tagline">PLC-style scan cycle on ESP-IDF and FreeRTOS — Modbus, sensors, MQTT, OTA, and recovery in a fixed module order every takt.</p>

<p class="quick-links">
  <a href="{{ '/developer_guide/' | relative_url }}">Quick Start</a> ·
  <a href="{{ '/architecture/' | relative_url }}">Architecture</a> ·
  <a href="{{ '/api_reference/' | relative_url }}">API Reference</a> ·
  <a href="https://github.com/Developer-RU/Takt-OS/releases">Releases</a> ·
  <a href="{{ site.repo_url }}">GitHub</a>
</p>

## Features

| Category | Capabilities |
|----------|--------------|
| **Kernel** | Takt scheduler, EventBus, TimerManager, overrun detection, diagnostics |
| **Drivers** | GPIO, UART, ADC, flash HAL, platform init |
| **Middleware** | Modbus RTU, WiFi, MQTT, BLE stub, WebServer, sensor/relay modules |
| **Services** | OTA, telemetry batching, remote config, Modbus gateway |
| **Recovery** | BLE / WiFi / UART DFU, dual-bank firmware cache, rollback |
| **Tooling** | ESP-IDF, PlatformIO, host GTest suite, CI for ESP32 + host builds |

## Quick Start

```bash
git clone https://github.com/Developer-RU/Takt-OS.git
cd Takt-OS/examples/demo_controller
source $IDF_PATH/export.sh
idf.py set-target esp32
idf.py build
idf.py -p PORT flash monitor
```

| Artifact | Path |
|----------|------|
| Reference app | `examples/demo_controller` |
| Recovery app | `examples/recovery_app` |
| Partition table | `examples/demo_controller/partitions.csv` |
| Prebuilt firmware | [GitHub Releases](https://github.com/Developer-RU/Takt-OS/releases) |

## Documentation

| Document | Description |
|----------|-------------|
| [Getting Started]({{ '/developer_guide/' | relative_url }}) | First module, registration, build |
| [PlatformIO / VS Code]({{ '/platformio_vscode/' | relative_url }}) | IDE integration and flash |
| [Architecture]({{ '/architecture/' | relative_url }}) | Layers, takt, flash map |
| [Scheduler]({{ '/scheduler/' | relative_url }}) | Module types, overrun budget |
| [Event Bus]({{ '/event_bus/' | relative_url }}) | Pub/sub and queued events |
| [Memory]({{ '/memory/' | relative_url }}) | NVS, storage, OTA cache |
| [Bootloader]({{ '/bootloader/' | relative_url }}) | Boot modes, validation |
| [Recovery]({{ '/recovery/' | relative_url }}) | DFU and rollback |
| [API Reference]({{ '/api_reference/' | relative_url }}) | Class and C API tables |
| [UML Diagrams]({{ '/uml/diagrams/' | relative_url }}) | Mermaid component views |
| [Components]({{ '/components/kernel/' | relative_url }}) | Per-layer reference |
| [FAQ]({{ '/faq/' | relative_url }}) | Common questions |
| [Licensing]({{ '/licensing/' | relative_url }}) | Apache 2.0 attribution |

## Tech Stack

- **MCU:** Espressif ESP32 (ESP32-S3 / ESP32-C3 configs available)
- **Runtime:** ESP-IDF 5.x, FreeRTOS (system tasks)
- **Language:** C++17, CMake 3.16+
- **Industrial:** Modbus RTU, MQTT telemetry, OTA with rollback
- **CI:** Host library + GTest, ESP-IDF firmware build

## Downloads

Firmware binaries and source archives are attached to **[GitHub Releases](https://github.com/Developer-RU/Takt-OS/releases)** (tag `v*`). There is no separate package registry — ESP32 firmware is distributed as release artifacts.

## License

[Apache License 2.0](https://github.com/Developer-RU/Takt-OS/blob/main/LICENSE) · Developer: **Masyukov Pavel** · [Sponsor](https://github.com/sponsors/Developer-RU)
