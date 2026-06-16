# TAKT OS Documentation

> **Developer:** Masyukov Pavel · [p.masyukov@gmail.com](mailto:p.masyukov@gmail.com) · **License:** [Apache License 2.0](https://github.com/Developer-RU/Takt-OS/blob/main/LICENSE)

Single source of project documentation. **GitHub Wiki** ([`wiki/Home.md`](wiki/Home.md)) is navigation-only with links here — no duplicate content.

## Getting started

| Document | Description |
|----------|-------------|
| [developer_guide.md](developer_guide.md) | First module, registration, build |
| [platformio_vscode.md](platformio_vscode.md) | VS Code / Cursor + PlatformIO |
| [faq.md](faq.md) | Frequently asked questions |

## Architecture

| Document | Description |
|----------|-------------|
| [architecture.md](architecture.md) | Layers, takt, flash map |
| [scheduler.md](scheduler.md) | Takt cycle, overrun, module types |
| [event_bus.md](event_bus.md) | Event bus |
| [timer_manager.md](timer_manager.md) | Software timers |
| [memory.md](memory.md) | NVS, storage, firmware cache |
| [uml/diagrams.md](uml/diagrams.md) | UML diagrams (Mermaid) |
| [implementation_plan.md](implementation_plan.md) | Phased implementation plan |

## Boot and recovery

| Document | Description |
|----------|-------------|
| [bootloader.md](bootloader.md) | Boot modes, validation |
| [recovery.md](recovery.md) | DFU, rollback |

## Code reference

| Document | Description |
|----------|-------------|
| [api_reference.md](api_reference.md) | API tables |
| [source_tree.md](source_tree.md) | Source file map |
| [components/kernel.md](components/kernel.md) | Kernel layer |
| [components/drivers.md](components/drivers.md) | Drivers layer |
| [components/middleware.md](components/middleware.md) | Middleware |
| [components/services.md](components/services.md) | Services |
| [components/recovery.md](components/recovery.md) | Recovery |
| [components/takt_boot.md](components/takt_boot.md) | Boot policy |
| [components/examples.md](components/examples.md) | Firmware examples |
| [components/sdk_lite.md](components/sdk_lite.md) | SDK and Lite |

## Quality and community

| Document | Description |
|----------|-------------|
| [certification.md](certification.md) | Test matrix |
| [licensing.md](licensing.md) | License and attribution |
| [modules.md](modules.md) | Module catalog |
| [../SECURITY.md](../SECURITY.md) | Security |
| [../CONTRIBUTING.md](../CONTRIBUTING.md) | Contributing |
| [../CHANGELOG.md](../CHANGELOG.md) | Version history |

## Build after cloning

```bash
git clone https://github.com/Developer-RU/Takt-OS.git
cd Takt-OS/examples/demo_controller
source $IDF_PATH/export.sh
idf.py set-target esp32 && idf.py build
```

CMake paths are **relative**. Do not commit `build/`, `.pio/`, or `sdkconfig`.

## Repository meta files

| File | Purpose |
|------|---------|
| [../LICENSE](../LICENSE) | Apache 2.0 |
| [../NOTICE](../NOTICE) | Third-party components |
| [../CODE_OF_CONDUCT.md](../CODE_OF_CONDUCT.md) | Code of conduct |

---

**TAKT OS** — Developer: **Masyukov Pavel** ([p.masyukov@gmail.com](mailto:p.masyukov@gmail.com)) · License: [Apache License 2.0](https://github.com/Developer-RU/Takt-OS/blob/main/LICENSE) · [Source](https://github.com/Developer-RU/Takt-OS)
