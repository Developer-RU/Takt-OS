# TAKT OS v0.2.0

**TAKT OS** — промышленная RTOS для ESP32 на концепции **такта** (Takt).

## Концепция

Каждый цикл ядра — один **такт**. Во время такта ядро последовательно вызывает обработчики всех зарегистрированных модулей. Модули делятся на три типа:

| Тип | Поведение | Пример |
|-----|-----------|--------|
| **Статический** | Фиксированный объём работы за такт | `readBytes(16)`, `processOneSample()` |
| **Динамический** | Сам определяет объём работы | Script Engine, парсеры |
| **Фоновый** | Работает только при наличии событий | BLE, OTA, очереди, сокеты |

## Структура проекта

```
TAKT-OS/
├── bootloader/     # Минимальный загрузчик, OTA/BLE recovery
├── recovery/       # DFU, откат прошивки
├── kernel/         # Ядро: Scheduler, EventBus, Timer, Memory
├── drivers/        # HAL-драйверы ESP32
├── middleware/     # UART, Modbus, MQTT, BLE, WiFi
├── services/       # OTA, NVS, Storage, Diagnostics
├── examples/
│   ├── wash_controller/    # WASH-PRO контроллер
│   └── recovery_app/       # Standalone recovery
├── sdk/                    # C API для сторонних модулей
├── lite/                   # TAKT OS Lite (урезанная версия)
├── targets/                # sdkconfig для esp32s3, esp32c3
├── docs/           # Архитектура, API, руководства
├── tools/          # Утилиты сборки и прошивки
└── tests/          # Модульные тесты
```

## Быстрый старт

### Требования

- ESP-IDF >= 5.0
- CMake >= 3.16
- Python 3.8+

### Сборка примера

```bash
cd examples/wash_controller
idf.py set-target esp32
idf.py build flash monitor
```

### Сборка через CMake (host-тесты)

```bash
mkdir build && cd build
cmake .. -DTAKT_BUILD_TESTS=ON
cmake --build .
ctest
```

## Документация

| Документ | Описание |
|----------|----------|
| [docs/architecture.md](docs/architecture.md) | Полная архитектура системы |
| [docs/kernel.md](docs/kernel.md) | Ядро TAKT OS |
| [docs/bootloader.md](docs/bootloader.md) | Загрузчик |
| [docs/recovery.md](docs/recovery.md) | Слой восстановления |
| [docs/scheduler.md](docs/scheduler.md) | Планировщик тактов |
| [docs/memory.md](docs/memory.md) | Управление памятью |
| [docs/event_bus.md](docs/event_bus.md) | Шина событий |
| [docs/timer_manager.md](docs/timer_manager.md) | Менеджер таймеров |
| [docs/api_reference.md](docs/api_reference.md) | Справочник API |
| [docs/developer_guide.md](docs/developer_guide.md) | Руководство разработчика |
| [docs/implementation_plan.md](docs/implementation_plan.md) | План реализации по этапам |
| [docs/uml/](docs/uml/) | UML-диаграммы |

## Лицензия

Proprietary — WASH-PRO / TAKT OS Project
