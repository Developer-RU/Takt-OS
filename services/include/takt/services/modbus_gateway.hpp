#pragma once

#include "takt/imodule.hpp"
#include "takt/modules/modbus_module.hpp"

namespace takt::services {

/// Modbus TCP gateway: bridges Modbus RTU to MQTT/TCP.
class ModbusGateway : public IModule {
public:
    bool init() override;
    void tick() override;
    void shutdown() override;
    const char* name() const override { return "ModbusGw"; }
    ModuleType type() const override { return ModuleType::Background; }
    bool hasWork() const override { return pollPending_; }

    void setModbusModule(modules::ModbusModule* modbus) { modbus_ = modbus; }

private:
    modules::ModbusModule* modbus_ = nullptr;
    bool pollPending_ = true;
    uint16_t pollAddr_ = 0;
};

} // namespace takt::services
