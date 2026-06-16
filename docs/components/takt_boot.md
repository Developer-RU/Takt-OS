# Boot policy (`takt_boot/`)

ESP-IDF component name: **`takt_boot`**

> Directory is named `takt_boot` (not `bootloader`) to avoid collision with ESP-IDF's built-in `bootloader` component.

Application-level boot policy running **after** the ESP-IDF second-stage bootloader. It does not replace the Espressif bootloader binary.

## Source

| File | Role |
|------|------|
| `src/bootloader.cpp` | Boot mode detection, validation, partition jump |
| `include/takt/bootloader.hpp` | `Bootloader`, `BootMode`, `BootConfig` |

## Boot modes

| Mode | Trigger | Action |
|------|---------|--------|
| `Normal` | Default | Run factory / OTA app |
| `Recovery` | GPIO0, boot count > 3 | Jump to recovery partition |
| `OtaPending` | NVS flag | Boot OTA slot |
| `FactoryReset` | Config | Erase NVS, recovery |
| `Emergency` | Hardware / RTC | Safe mode |

## RTC config

`BootConfig` in RTC memory: `bootCount`, flags — survives soft reset.

## Recovery partition lookup

```cpp
esp_partition_find_first(
    ESP_PARTITION_TYPE_APP,
    ESP_PARTITION_SUBTYPE_APP_TEST,  // subtype "test", not "factory"
    "recovery");
```

**Important:** Only one `factory` subtype partition is allowed — recovery uses `test` subtype. See [partition table](../../tools/partitions.csv).

## Depends on

`kernel`, ESP-IDF `esp_ota_ops`, `esp_partition`

## See also

- [bootloader.md](../bootloader.md)
- [recovery.md](../recovery.md)

---

**TAKT OS** — Developer: **Masyukov Pavel** ([p.masyukov@gmail.com](mailto:p.masyukov@gmail.com)) · License: [Apache License 2.0](https://github.com/Developer-RU/Takt-OS/blob/main/LICENSE) · [Source](https://github.com/Developer-RU/Takt-OS)
