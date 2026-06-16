// Copyright 2024-2026 Masyukov Pavel <p.masyukov@gmail.com>
// SPDX-License-Identifier: Apache-2.0
//
// TAKT OS — https://github.com/Developer-RU/Takt-OS

#include "takt/scheduler.hpp"
#include "takt/event_bus.hpp"
#include "takt/timer_manager.hpp"
#include "takt/logger.hpp"
#include "takt/diagnostics.hpp"

#ifdef TAKT_ESP32
#include "esp_timer.h"
#include "esp_task_wdt.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#else
#include <chrono>
#endif

namespace takt {

namespace {
constexpr const char* TAG = "Scheduler";
}

uint64_t Scheduler::nowUs() const {
#ifdef TAKT_ESP32
    return static_cast<uint64_t>(esp_timer_get_time());
#else
    using clock = std::chrono::steady_clock;
    static auto start = clock::now();
    auto elapsed = clock::now() - start;
    return std::chrono::duration_cast<std::chrono::microseconds>(elapsed).count();
#endif
}

ModuleId Scheduler::registerModule(IModule* module) {
    if (!module) return kInvalidModuleId;
    if (moduleCount_ >= kMaxModules) {
        TAKT_LOGE(TAG, "Module pool exhausted");
        return kInvalidModuleId;
    }
    auto& entry = modules_[moduleCount_];
    entry.module = module;
    entry.active = true;
    entry.stats.state = ModuleState::Registered;
    TAKT_LOGI(TAG, "Registered module '%s' [%s]",
              module->name(),
              module->type() == ModuleType::Static ? "static" :
              module->type() == ModuleType::Dynamic ? "dynamic" : "background");
    return static_cast<ModuleId>(moduleCount_++);
}

void Scheduler::unregisterModule(ModuleId id) {
    if (id >= moduleCount_) return;
    if (modules_[id].module) {
        modules_[id].module->shutdown();
    }
    modules_[id].active = false;
    modules_[id].stats.state = ModuleState::Shutdown;
}

bool Scheduler::initAll() {
    bool ok = true;
    for (size_t i = 0; i < moduleCount_; ++i) {
        auto& entry = modules_[i];
        if (!entry.active || !entry.module) continue;
        entry.stats.state = ModuleState::Initializing;
        TAKT_LOGI(TAG, "Initializing '%s'", entry.module->name());
        if (!entry.module->init()) {
            TAKT_LOGE(TAG, "Module '%s' init failed", entry.module->name());
            entry.stats.state = ModuleState::Error;
            ok = false;
        } else {
            entry.stats.state = ModuleState::Ready;
        }
    }
    return ok;
}

bool Scheduler::shouldRunModule(const ModuleEntry& entry) const {
    if (!entry.active || !entry.module) return false;
    if (entry.stats.state != ModuleState::Ready &&
        entry.stats.state != ModuleState::Running) return false;
    if (entry.module->type() == ModuleType::Background) {
        return entry.module->hasWork();
    }
    return true;
}

void Scheduler::runTakt() {
    const uint64_t taktStart = nowUs();

    TimerManager::instance().tick(taktPeriodMs_);
    EventBus::instance().dispatchQueued();

#ifdef TAKT_ESP32
    esp_task_wdt_reset();
#endif

    for (size_t i = 0; i < moduleCount_; ++i) {
        auto& entry = modules_[i];
        if (!shouldRunModule(entry)) {
            if (entry.module && entry.module->type() == ModuleType::Background) {
                ++entry.stats.skipCount;
            }
            continue;
        }

        const uint64_t modStart = nowUs();
        entry.stats.state = ModuleState::Running;
        entry.module->tick();
        const uint64_t modElapsed = nowUs() - modStart;

        entry.stats.tickCount++;
        entry.stats.lastUs = modElapsed;
        entry.stats.totalUs += modElapsed;
        if (modElapsed > entry.stats.maxUs) entry.stats.maxUs = modElapsed;

        const uint32_t budget = entry.module->budgetUs();
        if (budget > 0 && modElapsed > budget) {
            ++entry.stats.overrunCount;
            TAKT_LOGW(TAG, "Module '%s' overrun: %llu us (budget %u us)",
                      entry.module->name(),
                      static_cast<unsigned long long>(modElapsed), budget);
        }
        entry.stats.state = ModuleState::Ready;
    }

    const uint64_t taktElapsed = nowUs() - taktStart;
    stats_.totalTakts++;
    stats_.lastTaktUs = taktElapsed;
    stats_.totalTaktUs += taktElapsed;
    if (taktElapsed > stats_.maxTaktUs) stats_.maxTaktUs = taktElapsed;
    stats_.registeredModules = static_cast<uint32_t>(moduleCount_);

    if (taktBudgetUs_ > 0 && taktElapsed > taktBudgetUs_) {
        ++stats_.overrunCount;
        EventBus::instance().publish(Event::TaktOverrun,
                                     static_cast<uint32_t>(taktElapsed),
                                     taktBudgetUs_);
        TAKT_LOGW(TAG, "Takt overrun: %llu us (budget %u us)",
                  static_cast<unsigned long long>(taktElapsed), taktBudgetUs_);
    }
}

void Scheduler::run() {
    TAKT_LOGI(TAG, "Entering takt loop (period %u ms)", taktPeriodMs_);
    while (true) {
        runTakt();
#ifdef TAKT_ESP32
        // Yield to IDLE task between takts
        vTaskDelay(pdMS_TO_TICKS(taktPeriodMs_));
#endif
    }
}

void Scheduler::shutdownAll() {
    for (size_t i = 0; i < moduleCount_; ++i) {
        if (modules_[i].active && modules_[i].module) {
            modules_[i].module->shutdown();
            modules_[i].stats.state = ModuleState::Shutdown;
        }
    }
}

const ModuleStats& Scheduler::moduleStats(ModuleId id) const {
    static ModuleStats empty{};
    if (id >= moduleCount_) return empty;
    return modules_[id].stats;
}

} // namespace takt
