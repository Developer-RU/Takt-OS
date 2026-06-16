# TAKT OS API Reference

## Kernel

### `takt::Kernel`

| Method | Return | Description |
|--------|--------|-------------|
| `instance()` | `Kernel&` | Singleton |
| `boot()` | `bool` | System initialization |
| `run()` | `void` | Infinite takt loop |
| `requestShutdown()` | `void` | Graceful shutdown |
| `scheduler()` | `Scheduler&` | Access to the scheduler |
| `eventBus()` | `EventBus&` | Access to the event bus |
| `timerManager()` | `TimerManager&` | Access to timers |
| `printStatistics()` | `void` | Print diagnostics |
| `collectStats()` | `KernelStats` | Collect statistics |
| `isRunning()` | `bool` | Running state |

## Scheduler

### `takt::Scheduler`

| Method | Return | Description |
|--------|--------|-------------|
| `registerModule(IModule*)` | `ModuleId` | Register a module |
| `unregisterModule(ModuleId)` | `void` | Unregister a module |
| `initAll()` | `bool` | Call init() for all modules |
| `runTakt()` | `void` | Run one takt cycle |
| `run()` | `void` | Infinite loop |
| `shutdownAll()` | `void` | Call shutdown() for all modules |
| `moduleStats(ModuleId)` | `const ModuleStats&` | Module statistics |
| `stats()` | `const SchedulerStats&` | Global statistics |
| `setTaktBudgetUs(uint32_t)` | `void` | Takt budget (µs) |
| `setTaktPeriodMs(uint32_t)` | `void` | Takt period (ms) |
| `moduleCount()` | `size_t` | Number of modules |

## IModule

### `takt::IModule`

| Method | Return | Description |
|--------|--------|-------------|
| `init()` | `bool` | Initialization |
| `tick()` | `void` | One takt cycle |
| `shutdown()` | `void` | Shutdown |
| `name()` | `const char*` | Module name |
| `type()` | `ModuleType` | Static/Dynamic/Background |
| `budgetUs()` | `uint32_t` | Time budget (default 0) |
| `hasWork()` | `bool` | Whether work is pending (default true) |

## EventBus

### `takt::EventBus`

| Method | Return | Description |
|--------|--------|-------------|
| `instance()` | `EventBus&` | Singleton |
| `subscribe(Event, callback, userData)` | `int` | Subscribe; returns handle or -1 |
| `unsubscribe(int handle)` | `void` | Unsubscribe |
| `publish(Event)` | `void` | Synchronous publish |
| `publish(Event, p1, p2)` | `void` | Publish with parameters |
| `enqueue(EventData)` | `bool` | Deferred publish |
| `dispatchQueued()` | `void` | Process the queue |

## Timer

### `takt::Timer`

| Method | Return | Description |
|--------|--------|-------------|
| `Timer(intervalMs, repeat)` | — | Constructor |
| `setInterval(ms)` | `void` | Set interval |
| `setRepeat(bool)` | `void` | One-shot / repeat |
| `onTimeout(callback)` | `void` | Register callback |
| `start()` | `void` | Start |
| `stop()` | `void` | Stop |
| `reset()` | `void` | Reset counter |
| `isActive()` | `bool` | Whether active |
| `isRepeat()` | `bool` | Repeat mode |

## StorageManager

| Method | Return | Description |
|--------|--------|-------------|
| `instance()` | `StorageManager&` | Singleton |
| `init(FlashBackend)` | `bool` | Initialization |
| `read(offset, buf, len)` | `int` | Read |
| `write(offset, buf, len)` | `int` | Write (auto-erase) |
| `eraseSector(offset)` | `int` | Erase sector |
| `registerRegion(name, offset, size)` | `bool` | Named region |
| `getRegion(name)` | `const FlashRegion*` | Region information |

## NvsManager

| Method | Return | Description |
|--------|--------|-------------|
| `init(namespace)` | `bool` | Initialization |
| `setBlob(key, data, len, ver)` | `bool` | Write blob |
| `getBlob(key, data, maxLen, ver)` | `int` | Read blob |
| `setU8/U16/U32(key, value, ver)` | `bool` | Write integer |
| `getU8/U16/U32(key, value)` | `bool` | Read integer |
| `setString(key, value, ver)` | `bool` | Write string |
| `getString(key, buf, maxLen)` | `bool` | Read string |
| `remove(key)` | `bool` | Delete key |
| `backup()` | `bool` | Backup |
| `restoreFromBackup()` | `bool` | Restore |
| `verifyIntegrity()` | `bool` | Integrity check |

## FirmwareCache

| Method | Return | Description |
|--------|--------|-------------|
| `init(slotA, slotB, size)` | `bool` | Initialization |
| `activeSlot()` | `uint8_t` | Current slot |
| `inactiveSlot()` | `uint8_t` | Slot for OTA |
| `beginWrite(size, version)` | `bool` | Begin write |
| `writeChunk(data, len)` | `int` | Write chunk |
| `finalizeWrite()` | `bool` | Finalize + CRC |
| `verify(slot)` | `bool` | Verify |
| `activateSlot(slot)` | `bool` | Activate |
| `rollback()` | `bool` | Rollback |

## Bootloader

| Method | Return | Description |
|--------|--------|-------------|
| `entry()` | `void` | Entry point |
| `determineBootMode()` | `BootMode` | Determine boot mode |
| `validateFirmware(slot)` | `bool` | Validate |
| `jumpToFirmware(offset)` | `void` | Jump |
| `markBootSuccessful()` | `void` | Reset bootCount |

## RecoveryManager

| Method | Return | Description |
|--------|--------|-------------|
| `init(channel)` | `bool` | Initialization |
| `startDfuListener()` | `bool` | Start DFU |
| `receiveChunk(data, len)` | `int` | Receive data |
| `finalizeDfu()` | `bool` | Finalize DFU |
| `abort()` | `void` | Cancel DFU |
| `rollback()` | `bool` | Rollback |

## Logging

```cpp
TAKT_LOGI("Tag", "format %d", value);  // Info
TAKT_LOGW("Tag", "warning");           // Warn
TAKT_LOGE("Tag", "error");             // Error
TAKT_LOGD("Tag", "debug %s", str);     // Debug
```

## Macros

```cpp
TAKT_PUBLISH(takt::Event::WiFiConnected);
TAKT_SUBSCRIBE(takt::Event::WiFiConnected, callback, userData);
```

## Component documentation

Per-layer file maps and design notes:

| Layer | Doc |
|-------|-----|
| Kernel | [components/kernel.md](components/kernel.md) |
| Drivers | [components/drivers.md](components/drivers.md) |
| Middleware | [components/middleware.md](components/middleware.md) |
| Services | [components/services.md](components/services.md) |
| Recovery | [components/recovery.md](components/recovery.md) |
| takt_boot | [components/takt_boot.md](components/takt_boot.md) |
| Examples | [components/examples.md](components/examples.md) |
| SDK / Lite | [components/sdk_lite.md](components/sdk_lite.md) |

## C SDK

See `sdk/include/takt_sdk.h` and [components/sdk_lite.md](components/sdk_lite.md).

## License

API documentation © 2024-2026 Masyukov Pavel. Code licensed under [Apache 2.0](../LICENSE). When reusing snippets, attribute the source — see [licensing.md](licensing.md).

---

**TAKT OS** — Developer: **Masyukov Pavel** ([p.masyukov@gmail.com](mailto:p.masyukov@gmail.com)) · License: [Apache License 2.0](https://github.com/Developer-RU/Takt-OS/blob/main/LICENSE) · [Source](https://github.com/Developer-RU/Takt-OS)
