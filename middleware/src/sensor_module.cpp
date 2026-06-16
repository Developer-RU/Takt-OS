// Copyright 2024-2026 Masyukov Pavel <p.masyukov@gmail.com>
// SPDX-License-Identifier: Apache-2.0
//
// TAKT OS — https://github.com/Developer-RU/Takt-OS

#include "takt/modules/sensor_module.hpp"
#include "takt/drivers/platform.hpp"
#include "takt/event_bus.hpp"
#include "takt/logger.hpp"

namespace takt::modules {

namespace {
constexpr const char* TAG = "Sensor";
}

bool SensorModule::init() {
    if (!drivers::Adc::init(0)) {
        TAKT_LOGW(TAG, "ADC init failed");
    }
    TAKT_LOGI(TAG, "Sensor init");
    return true;
}

void SensorModule::processOneSample() {
    lastValue_ = drivers::Adc::readVoltage(0);
    if (lastValue_ < 0) lastValue_ = 0;
    ++sampleCount_;
}

void SensorModule::tick() {
    processOneSample();
    EventBus::instance().publish(Event::SensorDataReady,
                                 static_cast<uint32_t>(lastValue_ * 100));
}

void SensorModule::shutdown() {
    TAKT_LOGI(TAG, "Sensor shutdown, %u samples", sampleCount_);
}

} // namespace takt::modules
