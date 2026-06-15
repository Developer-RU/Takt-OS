# Changelog

All notable changes to **TAKT OS** are documented in this file.

The format is based on [Keep a Changelog](https://keepachangelog.com/en/1.1.0/),
and this project adheres to [Semantic Versioning](https://semver.org/spec/v2.0.0.html).

## [Unreleased]

### Added
- Apache 2.0 license, NOTICE, CONTRIBUTING, SECURITY, GitHub wiki docs
- PlatformIO / VS Code integration (`platformio.ini`, `.vscode/`, workspace)
- Public documentation hub and component reference under `docs/`

### Changed
- Вики: только русское оглавление (`docs/wiki/`), без дублирования `docs/`; FAQ перенесён в `docs/faq.md`
- Renamed example `wash_controller` → `demo_controller`; removed WASH-PRO branding
- Events `WashCycleStart/Complete` → `CycleStarted` / `CycleCompleted`
- GitHub repository URL: https://github.com/Developer-RU/Takt-OS
- Removed committed `sdkconfig` (machine-specific paths); use `sdkconfig.defaults` only
- `.gitignore` excludes `build/`, `.pio/`, `sdkconfig` artifacts
- VS Code tasks use `$IDF_PATH` and `pio` from PATH (no user home paths)
- Recovery partition subtype: `factory` → `test` (fixes ESP-IDF bootloader factory overwrite)
- Main task stack size increased to 12 KB for `demo_controller`

## [0.2.0] - 2026-06-15

### Added
- Full layered architecture: kernel, drivers, middleware, services, recovery, `takt_boot`
- **Kernel:** takt scheduler, EventBus, TimerManager, StorageManager, NVS, FirmwareCache, diagnostics
- **Drivers:** GPIO, UART, ADC, flash partition backend, platform init
- **Middleware:** UART, Sensor, Relay, Modbus RTU, WiFi, MQTT, BLE (stub), WebServer
- **Services:** OTA, Telemetry, ConfigManager, ModbusGateway
- **Examples:** `demo_controller` (reference demo), `recovery_app`
- **Recovery:** BLE / WiFi / UART DFU transports, rollback
- **SDK:** C API (`takt_sdk.h`) for third-party modules
- **Lite:** reduced kernel skeleton for constrained targets
- **Targets:** `sdkconfig.defaults` for ESP32-S3, ESP32-C3
- **Tools:** partition table, stress test, manufacturing test scripts
- **Tests:** host GTest suite (scheduler, event bus, timer, NVS, firmware cache, bootloader)
- **CI:** GitHub Actions (host CMake + ESP-IDF container build)
- Documentation: architecture, API reference, UML diagrams, implementation plan, certification

### Fixed
- ESP-IDF component naming (`REQUIRES` matches folder names)
- `bootloader/` renamed to `takt_boot/` (ESP-IDF name collision)
- Partition table ESP-IDF compatibility
- Multiple compile fixes for ESP-IDF 5.5 (ADC, snprintf, esp_image metadata, etc.)

### Known limitations
- BLE NimBLE integration requires `CONFIG_BT_NIMBLE_ENABLED`
- Recovery partition (256 KB) too small for full `demo_controller` binary
- 30-day hardware soak test not yet completed
- Secure Boot / flash encryption not enabled

## [0.1.0] - 2026-06-01

### Added
- Initial project skeleton
- Core `IModule` interface and scheduler prototype
- Basic documentation structure

[Unreleased]: https://github.com/Developer-RU/Takt-OS/compare/v0.2.0...HEAD
[0.2.0]: https://github.com/Developer-RU/Takt-OS/compare/v0.1.0...v0.2.0
[0.1.0]: https://github.com/Developer-RU/Takt-OS/releases/tag/v0.1.0
