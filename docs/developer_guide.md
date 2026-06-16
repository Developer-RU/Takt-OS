# TAKT OS — Developer Guide

## 1. Creating a Module

### Step 1: Define the Class

```cpp
#include "takt/imodule.hpp"

class MyModule : public takt::IModule {
public:
    bool init() override {
        // Initialize hardware, allocate buffers
        return true;
    }

    void tick() override {
        // Work for one takt cycle
    }

    void shutdown() override {
        // Release resources
    }

    const char* name() const override { return "MyModule"; }
    takt::ModuleType type() const override { return takt::ModuleType::Static; }
    uint32_t budgetUs() const override { return 1000; }
};
```

### Step 2: Choose a Module Type

| Type | When to Use | Implement |
|------|-------------|-----------|
| `Static` | Fixed workload (UART, ADC, GPIO) | `budgetUs()` |
| `Dynamic` | Variable load (parser, script) | `tick()` with a loop |
| `Background` | Event waiting (BLE, OTA) | `hasWork()` |

### Step 3: Register

```cpp
MyModule myModule;
kernel.scheduler().registerModule(&myModule);
```

## 2. Creating an ESP-IDF Project

### Structure

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
set(TAKT_OS_ROOT "${CMAKE_CURRENT_LIST_DIR}/../..")
list(APPEND EXTRA_COMPONENT_DIRS
    "${TAKT_OS_ROOT}/kernel"
    "${TAKT_OS_ROOT}/drivers"
    "${TAKT_OS_ROOT}/middleware"
    "${TAKT_OS_ROOT}/services"
    "${TAKT_OS_ROOT}/recovery"
    "${TAKT_OS_ROOT}/takt_boot"
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

## 3. Working with Events

```cpp
// Publish from a module
void SensorModule::tick() {
    processOneSample();
    takt::EventData ev{};
    ev.id = takt::Event::SensorDataReady;
    ev.param1 = static_cast<uint32_t>(lastValue_ * 100);
    takt::EventBus::instance().enqueue(ev);
}

// Subscribe in main
takt::EventBus::instance().subscribe(
    takt::Event::SensorDataReady,
    [](const takt::EventData& data, void* ctx) {
        // handle sensor data
    },
    nullptr
);
```

## 4. Working with NVS

```cpp
auto& nvs = takt::NvsManager::instance();

// Save configuration
struct Config { uint32_t cycleTime; uint32_t pressure; };
Config cfg{120, 150};
nvs.setBlob("app_config", &cfg, sizeof(cfg), /*version=*/1);

// Read with version check
uint16_t ver;
Config loaded{};
if (nvs.getBlob("app_config", &loaded, sizeof(loaded), &ver) > 0) {
    if (ver == 1) { /* use config */ }
}
```

## 5. OTA Update

```cpp
#include "takt/services/ota_service.hpp"

takt::services::OtaService ota;
kernel.scheduler().registerModule(&ota);

// Start OTA
ota.startUpdate(takt::services::OtaTransport::WiFi, imageSize, newVersion);

// Progress
ota.onProgress([](uint32_t rx, uint32_t total) {
    printf("OTA: %u%%\n", rx * 100 / total);
});
```

## 6. Debugging and Profiling

```cpp
// Periodic statistics output
takt::Timer diagTimer(10000, true);
diagTimer.onTimeout([]() {
    takt::Kernel::instance().printStatistics();
    takt::Diagnostics::instance().printReport();
});
diagTimer.start();
```

## 7. Coding Conventions

- C++17, no exceptions in the hot path
- Naming: `PascalCase` for classes, `camelCase` for methods
- Namespace: `takt::` for the kernel, `takt::modules::` for middleware, `takt::services::` for services
- Logging: `TAKT_LOGI/W/E/D("Tag", ...)`
- Do not block in `tick()` — it breaks takt determinism
- Background modules: always implement `hasWork()` correctly
- Events in `tick()`: use `enqueue()`, not `publish()`

## 8. Partition Table

Use `tools/partitions.csv` as a template. Two OTA slots are required for OTA:

```
ota_0, app, ota_0, 0x50000, 0x180000
ota_1, app, ota_1, 0x1D0000, 0x180000
```

## 9. Build and Flash

```bash
cd examples/demo_controller
idf.py set-target esp32
idf.py build
idf.py -p /dev/ttyUSB0 flash monitor
```

## 10. Host Tests

```bash
mkdir build && cd build
cmake .. -DTAKT_BUILD_TESTS=ON
cmake --build .
ctest --output-on-failure
```

---

**TAKT OS** — Developer: **Masyukov Pavel** ([p.masyukov@gmail.com](mailto:p.masyukov@gmail.com)) · License: [Apache License 2.0](https://github.com/Developer-RU/Takt-OS/blob/main/LICENSE) · [Source](https://github.com/Developer-RU/Takt-OS)
