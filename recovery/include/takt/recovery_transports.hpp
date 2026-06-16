// Copyright 2024-2026 Masyukov Pavel <p.masyukov@gmail.com>
// SPDX-License-Identifier: Apache-2.0
//
// TAKT OS — https://github.com/Developer-RU/Takt-OS

#pragma once

#include <cstddef>
#include <cstdint>
#include <functional>

namespace takt::recovery {

using DfuDataCallback = std::function<int(const void* data, size_t len)>;

/// BLE DFU transport (NimBLE GATT).
class BleDfu {
public:
    bool init();
    bool start();
    void stop();
    void setDataHandler(DfuDataCallback handler);
    bool isActive() const { return active_; }

private:
    bool active_ = false;
    DfuDataCallback dataHandler_;
};

/// WiFi OTA HTTP server transport.
class WiFiOta {
public:
    bool init(uint16_t port = 8080);
    bool start();
    void stop();
    void setDataHandler(DfuDataCallback handler);
    bool isActive() const { return active_; }

private:
    bool active_ = false;
    uint16_t port_ = 8080;
    DfuDataCallback dataHandler_;
};

/// UART manufacturing DFU transport.
class UartDfu {
public:
    bool init(int port = 0);
    bool start();
    void tick();
    void setDataHandler(DfuDataCallback handler);
    bool isActive() const { return active_; }

private:
    bool active_ = false;
    int port_ = 0;
    DfuDataCallback dataHandler_;
    enum class State { Idle, Header, Data, Done } state_ = State::Idle;
    uint32_t expectedSize_ = 0;
    uint32_t received_ = 0;
};

} // namespace takt::recovery
