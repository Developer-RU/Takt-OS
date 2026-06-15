# TAKT OS

**TAKT OS** — промышленная платформа для ESP32 с **тактовым** планировщиком: каждый цикл ядро последовательно вызывает модули в фиксированном порядке.

> Полная документация хранится в репозитории в папке [`docs/`](https://github.com/Developer-RU/Takt-OS/tree/main/docs).  
> **Вики не дублирует тексты** — здесь только навигация и быстрые ссылки.

Репозиторий: https://github.com/Developer-RU/Takt-OS  
Версия: **v0.2.0** · [CHANGELOG](https://github.com/Developer-RU/Takt-OS/blob/main/CHANGELOG.md)

---

## Быстрый старт

```bash
git clone https://github.com/Developer-RU/Takt-OS.git
cd Takt-OS/examples/demo_controller
source $IDF_PATH/export.sh
idf.py set-target esp32
idf.py build
idf.py -p PORT flash monitor
```

Подробнее: [Руководство разработчика](https://github.com/Developer-RU/Takt-OS/blob/main/docs/developer_guide.md) · [PlatformIO / VS Code](https://github.com/Developer-RU/Takt-OS/blob/main/docs/platformio_vscode.md)

---

## Оглавление документации

### Начало работы

| Документ | Описание |
|----------|----------|
| [developer_guide.md](https://github.com/Developer-RU/Takt-OS/blob/main/docs/developer_guide.md) | Первый модуль, регистрация, сборка |
| [platformio_vscode.md](https://github.com/Developer-RU/Takt-OS/blob/main/docs/platformio_vscode.md) | Сборка через PlatformIO |
| [faq.md](https://github.com/Developer-RU/Takt-OS/blob/main/docs/faq.md) | Частые вопросы |

### Архитектура

| Документ | Описание |
|----------|----------|
| [architecture.md](https://github.com/Developer-RU/Takt-OS/blob/main/docs/architecture.md) | Слои, такт, карта flash |
| [scheduler.md](https://github.com/Developer-RU/Takt-OS/blob/main/docs/scheduler.md) | Планировщик, overrun |
| [event_bus.md](https://github.com/Developer-RU/Takt-OS/blob/main/docs/event_bus.md) | Шина событий |
| [timer_manager.md](https://github.com/Developer-RU/Takt-OS/blob/main/docs/timer_manager.md) | Таймеры |
| [memory.md](https://github.com/Developer-RU/Takt-OS/blob/main/docs/memory.md) | NVS, storage, OTA cache |
| [uml/diagrams.md](https://github.com/Developer-RU/Takt-OS/blob/main/docs/uml/diagrams.md) | UML-диаграммы |

### Загрузка и восстановление

| Документ | Описание |
|----------|----------|
| [bootloader.md](https://github.com/Developer-RU/Takt-OS/blob/main/docs/bootloader.md) | Режимы загрузки |
| [recovery.md](https://github.com/Developer-RU/Takt-OS/blob/main/docs/recovery.md) | DFU, откат |

### Справочник кода

| Документ | Описание |
|----------|----------|
| [api_reference.md](https://github.com/Developer-RU/Takt-OS/blob/main/docs/api_reference.md) | API классов |
| [source_tree.md](https://github.com/Developer-RU/Takt-OS/blob/main/docs/source_tree.md) | Карта исходников |
| [components/](https://github.com/Developer-RU/Takt-OS/tree/main/docs/components) | Документация по слоям |

### Качество и лицензия

| Документ | Описание |
|----------|----------|
| [implementation_plan.md](https://github.com/Developer-RU/Takt-OS/blob/main/docs/implementation_plan.md) | План реализации |
| [certification.md](https://github.com/Developer-RU/Takt-OS/blob/main/docs/certification.md) | Сертификация |
| [licensing.md](https://github.com/Developer-RU/Takt-OS/blob/main/docs/licensing.md) | Лицензия и атрибуция |
| [SECURITY.md](https://github.com/Developer-RU/Takt-OS/blob/main/SECURITY.md) | Безопасность |
| [CONTRIBUTING.md](https://github.com/Developer-RU/Takt-OS/blob/main/CONTRIBUTING.md) | Участие в проекте |

---

## Примеры прошивок

| Пример | Описание |
|--------|----------|
| [`demo_controller`](https://github.com/Developer-RU/Takt-OS/tree/main/examples/demo_controller) | Эталонное приложение (UART, Modbus, WiFi, MQTT, OTA) |
| [`recovery_app`](https://github.com/Developer-RU/Takt-OS/tree/main/examples/recovery_app) | Прошивка recovery-раздела |
