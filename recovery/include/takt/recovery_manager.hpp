// Copyright 2024-2026 Masyukov Pavel <p.masyukov@gmail.com>
// SPDX-License-Identifier: Apache-2.0
//
// TAKT OS — https://github.com/Developer-RU/Takt-OS

#pragma once

#include "takt/recovery_transports.hpp"
#include <cstdint>
#include <functional>
#include <memory>

namespace takt::recovery {

class BleDfu;
class WiFiOta;
class UartDfu;

enum class RecoveryChannel : uint8_t {
    None = 0,
    Ble  = 1,
    WiFi = 2,
    Uart = 3,
};

enum class DfuState : uint8_t {
    Idle,
    AwaitingImage,
    Receiving,
    Verifying,
    Installing,
    Complete,
    Failed,
    RollingBack,
};

using DfuProgressCallback = std::function<void(uint32_t, uint32_t, DfuState)>;

class RecoveryManager {
public:
    static RecoveryManager& instance() {
        static RecoveryManager mgr;
        return mgr;
    }

    bool init(RecoveryChannel channel);
    bool startDfuListener();
    int receiveChunk(const void* data, size_t len);
    bool finalizeDfu();
    bool rollback();
    DfuState state() const { return state_; }
    void onProgress(DfuProgressCallback callback) { progressCb_ = std::move(callback); }
    void abort();
    void tick();
    void run();

private:
    DfuState            state_      = DfuState::Idle;
    RecoveryChannel     channel_    = RecoveryChannel::None;
    uint32_t            received_   = 0;
    uint32_t            totalSize_  = 0;
    DfuProgressCallback progressCb_;

    std::unique_ptr<BleDfu>  bleDfu_;
    std::unique_ptr<WiFiOta> wifiOta_;
    std::unique_ptr<UartDfu> uartDfu_;

    void setState(DfuState newState);
    void notifyProgress();
};

} // namespace takt::recovery
