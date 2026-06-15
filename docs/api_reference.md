# TAKT OS API Reference

## Kernel

### `takt::Kernel`

| Method | Return | Description |
|--------|--------|-------------|
| `instance()` | `Kernel&` | Singleton |
| `boot()` | `bool` | Инициализация системы |
| `run()` | `void` | Бесконечный цикл тактов |
| `requestShutdown()` | `void` | Graceful shutdown |
| `scheduler()` | `Scheduler&` | Доступ к планировщику |
| `eventBus()` | `EventBus&` | Доступ к шине событий |
| `timerManager()` | `TimerManager&` | Доступ к таймерам |
| `printStatistics()` | `void` | Вывод диагностики |
| `collectStats()` | `KernelStats` | Сбор статистики |
| `isRunning()` | `bool` | Состояние |

## Scheduler

### `takt::Scheduler`

| Method | Return | Description |
|--------|--------|-------------|
| `registerModule(IModule*)` | `ModuleId` | Регистрация модуля |
| `unregisterModule(ModuleId)` | `void` | Удаление модуля |
| `initAll()` | `bool` | init() для всех модулей |
| `runTakt()` | `void` | Один такт |
| `run()` | `void` | Бесконечный цикл |
| `shutdownAll()` | `void` | shutdown() для всех |
| `moduleStats(ModuleId)` | `const ModuleStats&` | Статистика модуля |
| `stats()` | `const SchedulerStats&` | Глобальная статистика |
| `setTaktBudgetUs(uint32_t)` | `void` | Бюджет такта (мкс) |
| `setTaktPeriodMs(uint32_t)` | `void` | Период такта (мс) |
| `moduleCount()` | `size_t` | Число модулей |

## IModule

### `takt::IModule`

| Method | Return | Description |
|--------|--------|-------------|
| `init()` | `bool` | Инициализация |
| `tick()` | `void` | Один такт |
| `shutdown()` | `void` | Завершение |
| `name()` | `const char*` | Имя модуля |
| `type()` | `ModuleType` | Static/Dynamic/Background |
| `budgetUs()` | `uint32_t` | Бюджет времени (default 0) |
| `hasWork()` | `bool` | Есть ли работа (default true) |

## EventBus

### `takt::EventBus`

| Method | Return | Description |
|--------|--------|-------------|
| `instance()` | `EventBus&` | Singleton |
| `subscribe(Event, callback, userData)` | `int` | Подписка, handle или -1 |
| `unsubscribe(int handle)` | `void` | Отписка |
| `publish(Event)` | `void` | Синхронная публикация |
| `publish(Event, p1, p2)` | `void` | С параметрами |
| `enqueue(EventData)` | `bool` | Отложенная публикация |
| `dispatchQueued()` | `void` | Обработка очереди |

## Timer

### `takt::Timer`

| Method | Return | Description |
|--------|--------|-------------|
| `Timer(intervalMs, repeat)` | — | Конструктор |
| `setInterval(ms)` | `void` | Установить интервал |
| `setRepeat(bool)` | `void` | One-shot / repeat |
| `onTimeout(callback)` | `void` | Регистрация callback |
| `start()` | `void` | Запуск |
| `stop()` | `void` | Остановка |
| `reset()` | `void` | Сброс счётчика |
| `isActive()` | `bool` | Активен ли |
| `isRepeat()` | `bool` | Режим повтора |

## StorageManager

| Method | Return | Description |
|--------|--------|-------------|
| `instance()` | `StorageManager&` | Singleton |
| `init(FlashBackend)` | `bool` | Инициализация |
| `read(offset, buf, len)` | `int` | Чтение |
| `write(offset, buf, len)` | `int` | Запись (auto-erase) |
| `eraseSector(offset)` | `int` | Стирание сектора |
| `registerRegion(name, offset, size)` | `bool` | Именованный регион |
| `getRegion(name)` | `const FlashRegion*` | Информация о регионе |

## NvsManager

| Method | Return | Description |
|--------|--------|-------------|
| `init(namespace)` | `bool` | Инициализация |
| `setBlob(key, data, len, ver)` | `bool` | Запись blob |
| `getBlob(key, data, maxLen, ver)` | `int` | Чтение blob |
| `setU8/U16/U32(key, value, ver)` | `bool` | Запись числа |
| `getU8/U16/U32(key, value)` | `bool` | Чтение числа |
| `setString(key, value, ver)` | `bool` | Запись строки |
| `getString(key, buf, maxLen)` | `bool` | Чтение строки |
| `remove(key)` | `bool` | Удаление ключа |
| `backup()` | `bool` | Резервное копирование |
| `restoreFromBackup()` | `bool` | Восстановление |
| `verifyIntegrity()` | `bool` | Проверка целостности |

## FirmwareCache

| Method | Return | Description |
|--------|--------|-------------|
| `init(slotA, slotB, size)` | `bool` | Инициализация |
| `activeSlot()` | `uint8_t` | Текущий слот |
| `inactiveSlot()` | `uint8_t` | Слот для OTA |
| `beginWrite(size, version)` | `bool` | Начало записи |
| `writeChunk(data, len)` | `int` | Запись блока |
| `finalizeWrite()` | `bool` | Завершение + CRC |
| `verify(slot)` | `bool` | Проверка |
| `activateSlot(slot)` | `bool` | Активация |
| `rollback()` | `bool` | Откат |

## Bootloader

| Method | Return | Description |
|--------|--------|-------------|
| `entry()` | `void` | Точка входа |
| `determineBootMode()` | `BootMode` | Определение режима |
| `validateFirmware(slot)` | `bool` | Валидация |
| `jumpToFirmware(offset)` | `void` | Переход |
| `markBootSuccessful()` | `void` | Сброс bootCount |

## RecoveryManager

| Method | Return | Description |
|--------|--------|-------------|
| `init(channel)` | `bool` | Инициализация |
| `startDfuListener()` | `bool` | Старт DFU |
| `receiveChunk(data, len)` | `int` | Приём данных |
| `finalizeDfu()` | `bool` | Завершение DFU |
| `rollback()` | `bool` | Откат |
| `abort()` | `void` | Отмена DFU |

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
