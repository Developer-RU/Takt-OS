/**
 * @file takt_sdk.h
 * @brief TAKT OS SDK — public C API for third-party modules.
 *
 * Include this header to build modules compatible with TAKT OS
 * without depending on internal kernel headers.
 */
#pragma once

#include <stdint.h>
#include <stdbool.h>
#include <stddef.h>

#ifdef __cplusplus
extern "C" {
#endif

typedef enum {
    TAKT_MODULE_STATIC = 0,
    TAKT_MODULE_DYNAMIC = 1,
    TAKT_MODULE_BACKGROUND = 2,
} takt_module_type_t;

typedef struct {
    const char* name;
    takt_module_type_t type;
    bool (*init)(void* ctx);
    void (*tick)(void* ctx);
    void (*shutdown)(void* ctx);
    bool (*has_work)(void* ctx);
    uint32_t budget_us;
    void* ctx;
} takt_module_t;

typedef enum {
    TAKT_EVT_WIFI_CONNECTED = 0x0100,
    TAKT_EVT_SENSOR_READY   = 0x0300,
    TAKT_EVT_OTA_COMPLETE   = 0x0202,
} takt_event_t;

typedef void (*takt_event_cb_t)(takt_event_t event, uint32_t p1, uint32_t p2, void* user);

/** Register a C module with the kernel scheduler. Returns module id or -1. */
int takt_register_module(const takt_module_t* module);

/** Subscribe to system events. */
int takt_subscribe(takt_event_t event, takt_event_cb_t cb, void* user);

/** Publish an event. */
void takt_publish(takt_event_t event, uint32_t p1, uint32_t p2);

/** Log helpers. */
void takt_log_info(const char* tag, const char* fmt, ...);
void takt_log_error(const char* tag, const char* fmt, ...);

/** NVS helpers. */
bool takt_nvs_set_u32(const char* key, uint32_t value);
bool takt_nvs_get_u32(const char* key, uint32_t* value);

#ifdef __cplusplus
}
#endif
