// Copyright 2024-2026 Masyukov Pavel <p.masyukov@gmail.com>
// SPDX-License-Identifier: Apache-2.0
//
// TAKT OS — https://github.com/Developer-RU/Takt-OS

#include "takt/services/modbus_gateway.hpp"
#include "takt/logger.hpp"

namespace takt::services {

namespace {
constexpr const char* TAG = "ModbusGw";
}

bool ModbusGateway::init() {
    TAKT_LOGI(TAG, "Modbus TCP gateway init");
    return true;
}

void ModbusGateway::tick() {
    if (!pollPending_ || !modbus_) return;
    uint16_t value = 0;
    modbus_->readHoldingRegister(1, pollAddr_, value);
    pollAddr_ = static_cast<uint16_t>((pollAddr_ + 1) % 10);
}

void ModbusGateway::shutdown() {
    TAKT_LOGI(TAG, "Modbus gateway shutdown");
}

} // namespace takt::services
