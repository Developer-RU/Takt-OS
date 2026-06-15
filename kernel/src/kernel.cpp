#include "takt/kernel.hpp"
#include "takt/logger.hpp"
#include "takt/diagnostics.hpp"
#include "takt/nvs_manager.hpp"

#ifdef TAKT_ESP32
#include "esp_system.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#else
#include <cstdio>
#endif

namespace takt {

namespace {
constexpr const char* TAG = "Kernel";
}

bool Kernel::boot() {
    TAKT_LOGI(TAG, "TAKT OS booting v0.2.0...");

#ifdef TAKT_ESP32
    bootTimeMs_ = static_cast<uint32_t>(xTaskGetTickCount() * portTICK_PERIOD_MS);
#else
    bootTimeMs_ = 0;
#endif

    if (!NvsManager::instance().init()) {
        TAKT_LOGW(TAG, "NVS init failed, continuing with defaults");
    } else {
        NvsManager::instance().backup();
    }

    EventBus::instance().publish(Event::SystemBoot);

    if (!scheduler_.initAll()) {
        TAKT_LOGE(TAG, "Module initialization failed");
        return false;
    }

    EventBus::instance().publish(Event::SystemReady);
    running_ = true;
    TAKT_LOGI(TAG, "TAKT OS ready");
    return true;
}

void Kernel::run() {
    if (!running_) {
        TAKT_LOGE(TAG, "Kernel not booted, call boot() first");
        return;
    }
    scheduler_.run();
}

void Kernel::requestShutdown() {
    shutdown_ = true;
    EventBus::instance().publish(Event::SystemShutdown);
    NvsManager::instance().backup();
    scheduler_.shutdownAll();
    running_ = false;
}

void Kernel::updateMemoryStats(KernelStats& stats) const {
    auto heap = Diagnostics::instance().heapInfo();
    stats.heapFree     = heap.totalFree;
    stats.heapMinFree  = heap.minFreeEver;
    stats.largestBlock = heap.largestBlock;
}

void Kernel::detectStackOverflow() {
    if (!Diagnostics::instance().checkStacks()) {
        EventBus::instance().publish(Event::StackOverflow);
        TAKT_LOGE(TAG, "Stack overflow detected!");
    }
}

KernelStats Kernel::collectStats() const {
    KernelStats stats{};
    stats.scheduler = scheduler_.stats();
    updateMemoryStats(stats);
#ifdef TAKT_ESP32
    stats.uptimeMs = static_cast<uint32_t>(xTaskGetTickCount() * portTICK_PERIOD_MS) - bootTimeMs_;
    stats.resetReason = static_cast<uint32_t>(esp_reset_reason());
#else
    stats.uptimeMs = 0;
#endif
    return stats;
}

void Kernel::printStatistics() const {
    auto stats = collectStats();
    TAKT_LOGI(TAG, "=== TAKT OS Statistics ===");
    TAKT_LOGI(TAG, "Uptime: %u ms, reset: %u", stats.uptimeMs, stats.resetReason);
    TAKT_LOGI(TAG, "Takts: %llu, max: %llu us, overruns: %u",
              static_cast<unsigned long long>(stats.scheduler.totalTakts),
              static_cast<unsigned long long>(stats.scheduler.maxTaktUs),
              stats.scheduler.overrunCount);
    TAKT_LOGI(TAG, "Heap free: %u, min: %u, largest block: %u",
              static_cast<unsigned>(stats.heapFree),
              static_cast<unsigned>(stats.heapMinFree),
              static_cast<unsigned>(stats.largestBlock));
    TAKT_LOGI(TAG, "Modules: %u", stats.scheduler.registeredModules);

    for (size_t i = 0; i < scheduler_.moduleCount(); ++i) {
        const auto& ms = scheduler_.moduleStats(static_cast<ModuleId>(i));
        TAKT_LOGI(TAG, "  [%u] ticks=%llu last=%llu us max=%llu us overruns=%u skips=%u",
                  static_cast<unsigned>(i),
                  static_cast<unsigned long long>(ms.tickCount),
                  static_cast<unsigned long long>(ms.lastUs),
                  static_cast<unsigned long long>(ms.maxUs),
                  ms.overrunCount, ms.skipCount);
    }
    TAKT_LOGI(TAG, "=== End Statistics ===");
}

} // namespace takt
