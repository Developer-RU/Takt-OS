# TAKT OS

**TAKT OS** is an industrial platform for ESP32 with a **takt** scheduler: each cycle, the kernel sequentially calls modules in a fixed order.

> **Developer:** Masyukov Pavel · [p.masyukov@gmail.com](mailto:p.masyukov@gmail.com) · **License:** [Apache License 2.0](https://github.com/Developer-RU/Takt-OS/blob/main/LICENSE)

> Full documentation lives in the repository under [`docs/`](https://github.com/Developer-RU/Takt-OS/tree/main/docs).  
> **The wiki does not duplicate content** — navigation and quick links only.

Repository: https://github.com/Developer-RU/Takt-OS  
Version: **v0.2.0** · [CHANGELOG](https://github.com/Developer-RU/Takt-OS/blob/main/CHANGELOG.md)

---

## Quick start

```bash
git clone https://github.com/Developer-RU/Takt-OS.git
cd Takt-OS/examples/demo_controller
source $IDF_PATH/export.sh
idf.py set-target esp32
idf.py build
idf.py -p PORT flash monitor
```

More: [Developer guide](https://github.com/Developer-RU/Takt-OS/blob/main/docs/developer_guide.md) · [PlatformIO / VS Code](https://github.com/Developer-RU/Takt-OS/blob/main/docs/platformio_vscode.md)

---

## Documentation index

### Getting started

| Document | Description |
|----------|-------------|
| [developer_guide.md](https://github.com/Developer-RU/Takt-OS/blob/main/docs/developer_guide.md) | First module, registration, build |
| [platformio_vscode.md](https://github.com/Developer-RU/Takt-OS/blob/main/docs/platformio_vscode.md) | Build with PlatformIO |
| [faq.md](https://github.com/Developer-RU/Takt-OS/blob/main/docs/faq.md) | Frequently asked questions |

### Architecture

| Document | Description |
|----------|-------------|
| [architecture.md](https://github.com/Developer-RU/Takt-OS/blob/main/docs/architecture.md) | Layers, takt, flash map |
| [scheduler.md](https://github.com/Developer-RU/Takt-OS/blob/main/docs/scheduler.md) | Scheduler, overrun |
| [event_bus.md](https://github.com/Developer-RU/Takt-OS/blob/main/docs/event_bus.md) | Event bus |
| [timer_manager.md](https://github.com/Developer-RU/Takt-OS/blob/main/docs/timer_manager.md) | Timers |
| [memory.md](https://github.com/Developer-RU/Takt-OS/blob/main/docs/memory.md) | NVS, storage, OTA cache |
| [uml/diagrams.md](https://github.com/Developer-RU/Takt-OS/blob/main/docs/uml/diagrams.md) | UML diagrams |

### Boot and recovery

| Document | Description |
|----------|-------------|
| [bootloader.md](https://github.com/Developer-RU/Takt-OS/blob/main/docs/bootloader.md) | Boot modes |
| [recovery.md](https://github.com/Developer-RU/Takt-OS/blob/main/docs/recovery.md) | DFU, rollback |

### Code reference

| Document | Description |
|----------|-------------|
| [api_reference.md](https://github.com/Developer-RU/Takt-OS/blob/main/docs/api_reference.md) | Class API |
| [source_tree.md](https://github.com/Developer-RU/Takt-OS/blob/main/docs/source_tree.md) | Source map |
| [components/](https://github.com/Developer-RU/Takt-OS/tree/main/docs/components) | Layer documentation |

### Quality and license

| Document | Description |
|----------|-------------|
| [implementation_plan.md](https://github.com/Developer-RU/Takt-OS/blob/main/docs/implementation_plan.md) | Implementation plan |
| [certification.md](https://github.com/Developer-RU/Takt-OS/blob/main/docs/certification.md) | Certification |
| [licensing.md](https://github.com/Developer-RU/Takt-OS/blob/main/docs/licensing.md) | License and attribution |
| [SECURITY.md](https://github.com/Developer-RU/Takt-OS/blob/main/SECURITY.md) | Security |
| [CONTRIBUTING.md](https://github.com/Developer-RU/Takt-OS/blob/main/CONTRIBUTING.md) | Contributing |

---

## Firmware examples

| Example | Description |
|---------|-------------|
| [`demo_controller`](https://github.com/Developer-RU/Takt-OS/tree/main/examples/demo_controller) | Reference application (UART, Modbus, WiFi, MQTT, OTA) |
| [`recovery_app`](https://github.com/Developer-RU/Takt-OS/tree/main/examples/recovery_app) | Recovery partition firmware |

---

**TAKT OS** — Developer: **Masyukov Pavel** ([p.masyukov@gmail.com](mailto:p.masyukov@gmail.com)) · License: [Apache License 2.0](https://github.com/Developer-RU/Takt-OS/blob/main/LICENSE) · [Source](https://github.com/Developer-RU/Takt-OS)
