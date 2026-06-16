// Copyright 2024-2026 Masyukov Pavel <p.masyukov@gmail.com>
// SPDX-License-Identifier: Apache-2.0
//
// TAKT OS — https://github.com/Developer-RU/Takt-OS

#include "takt_sdk.h"
#include "takt/kernel.hpp"
#include "takt/event_bus.hpp"
#include "takt/nvs_manager.hpp"
#include "takt/logger.hpp"
#include <cstdio>
#include <cstdarg>

namespace {

class SdkModuleAdapter : public takt::IModule {
public:
    explicit SdkModuleAdapter(const takt_module_t& desc) : desc_(desc) {}

    bool init() override { return desc_.init ? desc_.init(desc_.ctx) : true; }
    void tick() override { if (desc_.tick) desc_.tick(desc_.ctx); }
    void shutdown() override { if (desc_.shutdown) desc_.shutdown(desc_.ctx); }
    const char* name() const override { return desc_.name ? desc_.name : "SdkModule"; }
    takt::ModuleType type() const override {
        switch (desc_.type) {
            case TAKT_MODULE_DYNAMIC: return takt::ModuleType::Dynamic;
            case TAKT_MODULE_BACKGROUND: return takt::ModuleType::Background;
            default: return takt::ModuleType::Static;
        }
    }
    uint32_t budgetUs() const override { return desc_.budget_us; }
    bool hasWork() const override {
        return desc_.has_work ? desc_.has_work(desc_.ctx) : true;
    }

private:
    takt_module_t desc_;
};

struct EventCtx {
    takt_event_cb_t cb;
    void* user;
};

void sdkEventBridge(const takt::EventData& data, void* user) {
    auto* ctx = static_cast<EventCtx*>(user);
    if (ctx && ctx->cb) {
        ctx->cb(static_cast<takt_event_t>(static_cast<uint32_t>(data.id)),
                data.param1, data.param2, ctx->user);
    }
}

} // namespace

extern "C" int takt_register_module(const takt_module_t* module) {
    if (!module) return -1;
    static SdkModuleAdapter adapter(*module);
    return static_cast<int>(takt::Kernel::instance().scheduler().registerModule(&adapter));
}

extern "C" int takt_subscribe(takt_event_t event, takt_event_cb_t cb, void* user) {
    static EventCtx ctx{cb, user};
    return takt::EventBus::instance().subscribe(
        static_cast<takt::Event>(event), sdkEventBridge, &ctx);
}

extern "C" void takt_publish(takt_event_t event, uint32_t p1, uint32_t p2) {
    takt::EventBus::instance().publish(static_cast<takt::Event>(event), p1, p2);
}

extern "C" void takt_log_info(const char* tag, const char* fmt, ...) {
    va_list args;
    va_start(args, fmt);
    char buf[256];
    vsnprintf(buf, sizeof(buf), fmt, args);
    va_end(args);
    takt::TAKT_LOGI(tag, "%s", buf);
}

extern "C" void takt_log_error(const char* tag, const char* fmt, ...) {
    va_list args;
    va_start(args, fmt);
    char buf[256];
    vsnprintf(buf, sizeof(buf), fmt, args);
    va_end(args);
    takt::TAKT_LOGE(tag, "%s", buf);
}

extern "C" bool takt_nvs_set_u32(const char* key, uint32_t value) {
    return takt::NvsManager::instance().setU32(key, value);
}

extern "C" bool takt_nvs_get_u32(const char* key, uint32_t* value) {
    if (!value) return false;
    return takt::NvsManager::instance().getU32(key, *value);
}
