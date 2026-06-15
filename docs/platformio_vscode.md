# VS Code / Cursor + PlatformIO

## Быстрый старт

1. Установите расширение **PlatformIO IDE** (`platformio.platformio-ide`).
2. Клонируйте репозиторий: `git clone https://github.com/Developer-RU/Takt-OS.git`
3. Откройте `Takt-OS.code-workspace` (или папку `examples/demo_controller`).
4. В боковой панели PlatformIO: **Build** → **Upload** → **Monitor**.

## Файлы конфигурации

| Файл | Назначение |
|------|------------|
| `examples/demo_controller/platformio.ini` | PlatformIO: demo_controller |
| `examples/recovery_app/platformio.ini` | PlatformIO: recovery (offset 0x110000) |
| `.vscode/extensions.json` | Рекомендует PlatformIO IDE |
| `.vscode/tasks.json` | Задачи ESP-IDF и PlatformIO |
| `Takt-OS.code-workspace` | Multi-root workspace |

## Порт ESP32

Автоопределение обычно работает. Иначе в `platformio.ini`:

```ini
upload_port = /dev/ttyUSB0
monitor_port = /dev/ttyUSB0
```

Windows: `COM3`. macOS: `/dev/cu.usbserial-*`. Список портов: `pio device list`.

Для задач VS Code **ESP-IDF: Flash** порт запрашивается при запуске.

## Сборка из терминала

```bash
# PlatformIO (путь клонирования без пробелов)
cd examples/demo_controller
pio run -t upload -t monitor

# ESP-IDF
source $IDF_PATH/export.sh
cd examples/demo_controller
idf.py set-target esp32
idf.py build
idf.py -p PORT flash monitor
```

## Не коммитить в git

- `build/`, `.pio/` — артефакты сборки
- `sdkconfig`, `sdkconfig.old` — содержат пути вашей машины

См. [.gitignore](../.gitignore).
