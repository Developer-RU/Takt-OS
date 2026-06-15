# TAKT OS — Руководство разработчика

## 1. Создание модуля

### Шаг 1: Определить класс

```cpp
#include "takt/imodule.hpp"

class MyModule : public takt::IModule {
public:
    bool init() override {
        // Инициализация hardware, alloc buffers
        return true;
    }

    void tick() override {
        // Работа за один такт
    }

    void shutdown() override {
        // Освобождение ресурсов
    }

    const char* name() const override { return "MyModule"; }
    takt::ModuleType type() const override { return takt::ModuleType::Static; }
    uint32_t budgetUs() const override { return 1000; }
};
```

### Шаг 2: Выбрать тип модуля

| Тип | Когда использовать | Реализовать |
|-----|-------------------|-------------|
| `Static` | Фиксированная работа (UART, ADC, GPIO) | `budgetUs()` |
| `Dynamic` | Переменная нагрузка (парсер, скрипт) | `tick()` с циклом |
| `Background` | Ожидание событий (BLE, OTA) | `hasWork()` |

### Шаг 3: Зарегистрировать

```cpp
MyModule myModule;
kernel.scheduler().registerModule(&myModule);
```

## 2. Создание ESP-IDF проекта

### Структура

```
my_project/
├── CMakeLists.txt          # include project.cmake
├── sdkconfig.defaults
└── main/
    ├── CMakeLists.txt      # idf_component_register
    └── main.cpp
```

### CMakeLists.txt (project)

```cmake
cmake_minimum_required(VERSION 3.16)
set(TAKT_OS_ROOT "/path/to/TAKT-OS")
list(APPEND EXTRA_COMPONENT_DIRS
    "${TAKT_OS_ROOT}/kernel"
    "${TAKT_OS_ROOT}/drivers"
    "${TAKT_OS_ROOT}/middleware"
    "${TAKT_OS_ROOT}/services"
)
include($ENV{IDF_PATH}/tools/cmake/project.cmake)
project(my_project)
```

### main.cpp

```cpp
#include "takt/kernel.hpp"
#include "takt/modules/uart_module.hpp"

extern "C" void app_main(void) {
    auto& kernel = takt::Kernel::instance();
    takt::modules::UartModule uart(0, 16);
    kernel.scheduler().registerModule(&uart);
    kernel.boot();
    kernel.run();
}
```

## 3. Работа с событиями

```cpp
// Публикация из модуля
void SensorModule::tick() {
    processOneSample();
    takt::EventData ev{};
    ev.id = takt::Event::SensorDataReady;
    ev.param1 = static_cast<uint32_t>(lastValue_ * 100);
    takt::EventBus::instance().enqueue(ev);
}

// Подписка в main
takt::EventBus::instance().subscribe(
    takt::Event::SensorDataReady,
    [](const takt::EventData& data, void* ctx) {
        // handle sensor data
    },
    nullptr
);
```

## 4. Работа с NVS

```cpp
auto& nvs = takt::NvsManager::instance();

// Сохранение конфигурации
struct Config { uint32_t washTime; uint32_t pressure; };
Config cfg{120, 150};
nvs.setBlob("wash_config", &cfg, sizeof(cfg), /*version=*/1);

// Чтение с проверкой версии
uint16_t ver;
Config loaded{};
if (nvs.getBlob("wash_config", &loaded, sizeof(loaded), &ver) > 0) {
    if (ver == 1) { /* use config */ }
}
```

## 5. OTA обновление

```cpp
#include "takt/services/ota_service.hpp"

takt::services::OtaService ota;
kernel.scheduler().registerModule(&ota);

// Запуск OTA
ota.startUpdate(takt::services::OtaTransport::WiFi, imageSize, newVersion);

// Прогресс
ota.onProgress([](uint32_t rx, uint32_t total) {
    printf("OTA: %u%%\n", rx * 100 / total);
});
```

## 6. Отладка и профилирование

```cpp
// Периодический вывод статистики
takt::Timer diagTimer(10000, true);
diagTimer.onTimeout([]() {
    takt::Kernel::instance().printStatistics();
    takt::Diagnostics::instance().printReport();
});
diagTimer.start();
```

## 7. Соглашения по коду

- C++17, без исключений в hot path
- Именование: `PascalCase` для классов, `camelCase` для методов
- Namespace: `takt::` для ядра, `takt::modules::` для middleware, `takt::services::` для сервисов
- Логирование: `TAKT_LOGI/W/E/D("Tag", ...)`
- Не блокировать в `tick()` — это нарушает детерминизм такта
- Фоновые модули: всегда реализуйте `hasWork()` корректно
- События в `tick()`: используйте `enqueue()`, не `publish()`

## 8. Partition Table

Используйте `tools/partitions.csv` как шаблон. Для OTA обязательны два OTA-слота:

```
ota_0, app, ota_0, 0x50000, 0x180000
ota_1, app, ota_1, 0x1D0000, 0x180000
```

## 9. Сборка и прошивка

```bash
cd examples/wash_controller
idf.py set-target esp32
idf.py build
idf.py -p /dev/ttyUSB0 flash monitor
```

## 10. Host-тесты

```bash
mkdir build && cd build
cmake .. -DTAKT_BUILD_TESTS=ON
cmake --build .
ctest --output-on-failure
```
