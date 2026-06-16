# VS Code / Cursor + PlatformIO

## Quick Start

1. Install the **PlatformIO IDE** extension (`platformio.platformio-ide`).
2. Clone the repository: `git clone https://github.com/Developer-RU/Takt-OS.git`
3. Open `Takt-OS.code-workspace` (or the `examples/demo_controller` folder).
4. In the PlatformIO sidebar: **Build** → **Upload** → **Monitor**.

## Configuration Files

| File | Purpose |
|------|---------|
| `examples/demo_controller/platformio.ini` | PlatformIO: demo_controller |
| `examples/recovery_app/platformio.ini` | PlatformIO: recovery (offset 0x110000) |
| `.vscode/extensions.json` | Recommends PlatformIO IDE |
| `.vscode/tasks.json` | ESP-IDF and PlatformIO tasks |
| `Takt-OS.code-workspace` | Multi-root workspace |

## ESP32 Port

Auto-detection usually works. Otherwise, in `platformio.ini`:

```ini
upload_port = /dev/ttyUSB0
monitor_port = /dev/ttyUSB0
```

Windows: `COM3`. macOS: `/dev/cu.usbserial-*`. List ports: `pio device list`.

For the VS Code **ESP-IDF: Flash** task, the port is prompted at launch.

## Building from the Terminal

```bash
# PlatformIO (clone path without spaces)
cd examples/demo_controller
pio run -t upload -t monitor

# ESP-IDF
source $IDF_PATH/export.sh
cd examples/demo_controller
idf.py set-target esp32
idf.py build
idf.py -p PORT flash monitor
```

## Do Not Commit to Git

- `build/`, `.pio/` — build artifacts
- `sdkconfig`, `sdkconfig.old` — contain machine-specific paths

See [.gitignore](../.gitignore).

---

**TAKT OS** — Developer: **Masyukov Pavel** ([p.masyukov@gmail.com](mailto:p.masyukov@gmail.com)) · License: [Apache License 2.0](https://github.com/Developer-RU/Takt-OS/blob/main/LICENSE) · [Source](https://github.com/Developer-RU/Takt-OS)
