# SDK & Lite

## C SDK (`sdk/`)

**Header:** `sdk/include/takt_sdk.h`

C API for third-party or legacy modules without including C++ kernel headers.

### Functions

| Function | Description |
|----------|-------------|
| `takt_register_module()` | Register C module vtable |
| `takt_subscribe()` / `takt_publish()` | EventBus from C |
| `takt_log_info()` / `takt_log_error()` | Logging |
| `takt_nvs_set_u32()` / `takt_nvs_get_u32()` | NVS access |

### Usage

Link `kernel` and include `takt_sdk.h`. Implement `takt_module_ops_t` with `init`, `tick`, `shutdown`.

## TAKT OS Lite (`lite/`)

Reduced footprint subset for memory-constrained targets (ESP32-C3, etc.).

| File | Role |
|------|------|
| `kernel_lite.cpp` | Minimal scheduler without full middleware stack |
| `CMakeLists.txt` | Optional component `TAKT_LITE` |

Enable via target `sdkconfig` or build flag when porting to small flash parts.

## Multi-target configs

| Path | Chip |
|------|------|
| `targets/esp32s3/sdkconfig.defaults` | ESP32-S3 |
| `targets/esp32c3/sdkconfig.defaults` | ESP32-C3 |

Copy relevant defaults into your example's `sdkconfig.defaults` when porting.

## See also

- [api_reference.md](../api_reference.md)
- [implementation_plan.md](../implementation_plan.md) — stage 6 multi-target
