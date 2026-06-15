# Частые вопросы (FAQ)

## Зачем TAKT OS, если в ESP-IDF уже есть FreeRTOS?

ESP-IDF — это **платформа** (драйверы, WiFi, flash, OTA). TAKT OS — **модель планирования приложения**: предсказуемый цикл сканирования, как в ПЛК. FreeRTOS остаётся для системных задач WiFi/TCP.

## Можно ли совмещать задачи FreeRTOS и TAKT OS?

Да. TAKT OS работает в `app_main`. Задачи ESP-IDF крутятся под капотом. Не блокируйте тактовый цикл надолго.

## Что такое overrun такта?

Планировщик измеряет длительность такта. Если она превышает `setTaktBudgetUs()`, пишется предупреждение и публикуется `Event::TaktOverrun`.

## Почему раздел recovery имеет subtype `test`?

В загрузчике ESP-IDF может быть только **один** раздел с subtype `factory`. Второй `factory` перезаписывал указатель на основное приложение. Recovery использует subtype `test`.

## PlatformIO падает из‑за пробела в пути

Клонируйте репозиторий в путь **без пробелов**, например `Takt-OS`.

## BLE не работает

Включите `CONFIG_BT_NIMBLE_ENABLED` в sdkconfig. `BleModule` — заготовка без полного GATT DFU.

## Прошивка не влезает в recovery

Так задумано: recovery — 256 KB. Собирайте `recovery_app`, а не полный `demo_controller`.

## Как указать атрибуцию TAKT OS в продукте?

См. [licensing.md](licensing.md) и [NOTICE](../NOTICE).

## Где справочник API?

[api_reference.md](api_reference.md)

## Что не коммитить в git?

`build/`, `.pio/`, `sdkconfig` — содержат пути вашей машины. См. [.gitignore](../.gitignore).
