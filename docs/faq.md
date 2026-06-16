# Frequently Asked Questions (FAQ)

## Why TAKT OS when ESP-IDF already has FreeRTOS?

ESP-IDF is the **platform** (drivers, WiFi, flash, OTA). TAKT OS is the **application scheduling model**: a predictable scan cycle, like a PLC. FreeRTOS remains for system tasks such as WiFi/TCP.

## Can FreeRTOS tasks and TAKT OS be used together?

Yes. TAKT OS runs in `app_main`. ESP-IDF tasks run under the hood. Do not block the takt loop for long periods.

## What is a takt overrun?

The scheduler measures takt duration. If it exceeds `setTaktBudgetUs()`, a warning is logged and `Event::TaktOverrun` is published.

## Why does the recovery partition have subtype `test`?

The ESP-IDF bootloader allows only **one** partition with subtype `factory`. A second `factory` partition overwrote the pointer to the main application. Recovery uses subtype `test`.

## PlatformIO fails because of a space in the path

Clone the repository into a path **without spaces**, for example `Takt-OS`.

## BLE does not work

Enable `CONFIG_BT_NIMBLE_ENABLED` in sdkconfig. `BleModule` is a stub without full GATT DFU.

## Firmware does not fit in recovery

This is by design: recovery is 256 KB. Build `recovery_app`, not the full `demo_controller`.

## How do I attribute TAKT OS in a product?

See [licensing.md](licensing.md) and [NOTICE](../NOTICE).

## Where is the API reference?

[api_reference.md](api_reference.md)

## What should not be committed to git?

`build/`, `.pio/`, `sdkconfig` — they contain machine-specific paths. See [.gitignore](../.gitignore).

---

**TAKT OS** — Developer: **Masyukov Pavel** ([p.masyukov@gmail.com](mailto:p.masyukov@gmail.com)) · License: [Apache License 2.0](https://github.com/Developer-RU/Takt-OS/blob/main/LICENSE) · [Source](https://github.com/Developer-RU/Takt-OS)
