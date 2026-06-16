// Copyright 2024-2026 Masyukov Pavel <p.masyukov@gmail.com>
// SPDX-License-Identifier: Apache-2.0
//
// TAKT OS — https://github.com/Developer-RU/Takt-OS

#include "takt/modules/modbus_module.hpp"
#include "takt/drivers/platform.hpp"
#include "takt/event_bus.hpp"
#include "takt/logger.hpp"

namespace takt::modules {

namespace {
constexpr const char* TAG = "Modbus";
}

ModbusModule::ModbusModule(Mode mode, int uartPort)
    : mode_(mode), uartPort_(uartPort) {}

bool ModbusModule::init() {
    if (mode_ == Mode::Rtu) {
        drivers::Uart::Config cfg{};
        cfg.port = uartPort_;
        cfg.baudRate = 9600;
        cfg.txPin = 17;
        cfg.rxPin = 16;
        drivers::Uart::init(cfg);
    }
    TAKT_LOGI(TAG, "Modbus init mode=%s", mode_ == Mode::Rtu ? "RTU" : "TCP");
    return true;
}

uint16_t ModbusModule::crc16(const uint8_t* data, size_t len) const {
    uint16_t crc = 0xFFFF;
    for (size_t i = 0; i < len; ++i) {
        crc ^= data[i];
        for (int j = 0; j < 8; ++j) {
            crc = (crc & 1) ? (crc >> 1) ^ 0xA001 : crc >> 1;
        }
    }
    return crc;
}

bool ModbusModule::readHoldingRegister(uint8_t slave, uint16_t addr, uint16_t& value) {
    uint8_t frame[8] = {
        slave, 0x03,
        static_cast<uint8_t>(addr >> 8), static_cast<uint8_t>(addr),
        0x00, 0x01,
    };
    const uint16_t crc = crc16(frame, 6);
    frame[6] = static_cast<uint8_t>(crc);
    frame[7] = static_cast<uint8_t>(crc >> 8);

    if (mode_ == Mode::Rtu) {
        drivers::Uart::write(uartPort_, frame, sizeof(frame));
        uint8_t resp[7]{};
        const int n = drivers::Uart::read(uartPort_, resp, sizeof(resp));
        if (n >= 5) value = static_cast<uint16_t>((resp[3] << 8) | resp[4]);
    }
    EventBus::instance().publish(Event::ModbusRequest, slave, addr);
    return true;
}

bool ModbusModule::writeCoil(uint8_t slave, uint16_t addr, bool value) {
    pendingSlave_ = slave;
    pendingAddr_ = addr;
    requestPending_ = true;
    (void)value;
    return true;
}

void ModbusModule::processRequest() {
    requestPending_ = false;
    uint16_t dummy = 0;
    readHoldingRegister(pendingSlave_, pendingAddr_, dummy);
}

void ModbusModule::tick() {
    if (requestPending_) processRequest();
}

void ModbusModule::shutdown() {
    TAKT_LOGI(TAG, "Modbus shutdown");
}

} // namespace takt::modules
