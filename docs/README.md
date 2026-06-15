# Документация TAKT OS

Единый источник документации проекта. **GitHub Wiki** ([`wiki/Home.md`](wiki/Home.md)) — только русскоязычное оглавление со ссылками сюда, без дублирования текстов.

## Начало работы

| Документ | Описание |
|----------|----------|
| [developer_guide.md](developer_guide.md) | Первый модуль, регистрация, сборка |
| [platformio_vscode.md](platformio_vscode.md) | VS Code / Cursor + PlatformIO |
| [faq.md](faq.md) | Частые вопросы |

## Архитектура

| Документ | Описание |
|----------|----------|
| [architecture.md](architecture.md) | Слои, такт, карта flash |
| [scheduler.md](scheduler.md) | Цикл такта, overrun, типы модулей |
| [event_bus.md](event_bus.md) | Шина событий |
| [timer_manager.md](timer_manager.md) | Программные таймеры |
| [memory.md](memory.md) | NVS, storage, кэш прошивки |
| [uml/diagrams.md](uml/diagrams.md) | UML-диаграммы (Mermaid) |
| [implementation_plan.md](implementation_plan.md) | План реализации по этапам |

## Загрузка и восстановление

| Документ | Описание |
|----------|----------|
| [bootloader.md](bootloader.md) | Режимы загрузки, валидация |
| [recovery.md](recovery.md) | DFU, откат |

## Справочник кода

| Документ | Описание |
|----------|----------|
| [api_reference.md](api_reference.md) | Таблицы API |
| [source_tree.md](source_tree.md) | Карта исходных файлов |
| [components/kernel.md](components/kernel.md) | Слой kernel |
| [components/drivers.md](components/drivers.md) | Слой drivers |
| [components/middleware.md](components/middleware.md) | Middleware |
| [components/services.md](components/services.md) | Services |
| [components/recovery.md](components/recovery.md) | Recovery |
| [components/takt_boot.md](components/takt_boot.md) | Политика загрузки |
| [components/examples.md](components/examples.md) | Примеры прошивок |
| [components/sdk_lite.md](components/sdk_lite.md) | SDK и Lite |

## Качество и сообщество

| Документ | Описание |
|----------|----------|
| [certification.md](certification.md) | Матрица тестов |
| [licensing.md](licensing.md) | Лицензия и атрибуция |
| [modules.md](modules.md) | Каталог модулей |
| [../SECURITY.md](../SECURITY.md) | Безопасность |
| [../CONTRIBUTING.md](../CONTRIBUTING.md) | Участие в разработке |
| [../CHANGELOG.md](../CHANGELOG.md) | История версий |

## Сборка после клонирования

```bash
git clone https://github.com/Developer-RU/Takt-OS.git
cd Takt-OS/examples/demo_controller
source $IDF_PATH/export.sh
idf.py set-target esp32 && idf.py build
```

Пути в CMake **относительные**. Не коммитьте `build/`, `.pio/`, `sdkconfig`.

## Метафайлы репозитория

| Файл | Назначение |
|------|------------|
| [../LICENSE](../LICENSE) | Apache 2.0 |
| [../NOTICE](../NOTICE) | Сторонние компоненты |
| [../CODE_OF_CONDUCT.md](../CODE_OF_CONDUCT.md) | Кодекс поведения |
