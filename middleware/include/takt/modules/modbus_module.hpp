#pragma once

#include "takt/imodule.hpp"
#include <cstdint>

namespace takt::modules {

/// Modbus RTU master/slave module.
class ModbusModule : public IModule {
public:
    enum class Mode { Rtu, Tcp };

    ModbusModule(Mode mode = Mode::Rtu, int uartPort = 1);

    bool init() override;
    void tick() override;
    void shutdown() override;
    const char* name() const override { return "Modbus"; }
    ModuleType type() const override { return ModuleType::Background; }
    bool hasWork() const override { return requestPending_; }

    bool readHoldingRegister(uint8_t slave, uint16_t addr, uint16_t& value);
    bool writeCoil(uint8_t slave, uint16_t addr, bool value);

private:
    Mode mode_;
    int uartPort_;
    bool requestPending_ = false;
    uint8_t pendingSlave_ = 0;
    uint16_t pendingAddr_ = 0;

    void processRequest();
    uint16_t crc16(const uint8_t* data, size_t len) const;
};

} // namespace takt::modules
