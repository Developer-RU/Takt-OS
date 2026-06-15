# Kernel (`kernel/`)

ESP-IDF component name: **`kernel`**

The kernel implements the takt scheduler, inter-module events, timers, persistence, and diagnostics. It runs inside the ESP-IDF `main` task (FreeRTOS); it does not replace FreeRTOS system tasks.

## Source layout

| File | Role |
|------|------|
| `src/kernel.cpp` | `Kernel` singleton: `boot()`, `run()`, shutdown |
| `src/scheduler.cpp` | Takt loop, module registration, overrun detection |
| `src/event_bus.cpp` | Sync/async event dispatch |
| `src/timer_manager.cpp` | Software timers tied to takt period |
| `src/storage_manager.cpp` | Named flash regions |
| `src/nvs_manager.cpp` | NVS key-value with backup |
| `src/firmware_cache.cpp` | Dual-bank OTA image staging |
| `src/cache_manager.cpp` | Ring buffer for telemetry |
| `src/diagnostics.cpp` | Stats collection |
| `src/crc32.cpp` | CRC32 for firmware integrity |

## Public headers (`include/takt/`)

| Header | Primary types |
|--------|----------------|
| `kernel.hpp` | `Kernel`, `KernelStats` |
| `scheduler.hpp` | `Scheduler`, `ModuleStats`, `SchedulerStats` |
| `imodule.hpp` | `IModule`, `ModuleType`, `ModuleState` |
| `types.hpp` | `ModuleId`, `kMaxModules`, aliases |
| `event.hpp` | `Event`, `EventData` |
| `event_bus.hpp` | `EventBus`, subscribe/publish macros |
| `timer.hpp` | `Timer` |
| `timer_manager.hpp` | `TimerManager` |
| `storage_manager.hpp` | `StorageManager`, `FlashRegion` |
| `nvs_manager.hpp` | `NvsManager` |
| `firmware_cache.hpp` | `FirmwareCache` |
| `cache_manager.hpp` | `CacheManager` |
| `diagnostics.hpp` | Diagnostic hooks |
| `logger.hpp` | `TAKT_LOGI/W/E/D` |
| `crc32.hpp` | `crc32()` |

## Lifecycle

```
Platform::init()
  → Kernel::boot()
      → Scheduler::initAll()   // each IModule::init()
  → Kernel::run()
      → loop: Scheduler::runTakt()
          → TimerManager::tick()
          → EventBus::dispatchQueued()
          → for each module: tick() / skip if background idle
          → overrun check vs setTaktBudgetUs()
```

## Configuration

| API | Default | Purpose |
|-----|---------|---------|
| `setTaktPeriodMs()` | 1 ms | Sleep between takts |
| `setTaktBudgetUs()` | 8000 µs | Warn if takt exceeds budget |
| `kMaxModules` | 48 | Registration limit |

## Dependencies

- **ESP32:** `esp_timer`, `esp_task_wdt`, FreeRTOS task delay
- **Host tests:** `std::chrono` stub for time

## See also

- [scheduler.md](../scheduler.md)
- [event_bus.md](../event_bus.md)
- [memory.md](../memory.md)
- [api_reference.md](../api_reference.md)
