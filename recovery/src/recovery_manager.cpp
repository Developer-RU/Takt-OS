#include "takt/recovery_manager.hpp"
#include "takt/recovery_transports.hpp"
#include "takt/firmware_cache.hpp"
#include "takt/event_bus.hpp"
#include "takt/bootloader.hpp"
#include "takt/logger.hpp"

#ifdef TAKT_ESP32
#include "esp_system.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#endif

namespace takt::recovery {

namespace {
constexpr const char* TAG = "Recovery";
}

void RecoveryManager::setState(DfuState newState) {
    state_ = newState;
    TAKT_LOGI(TAG, "DFU state -> %u", static_cast<uint8_t>(newState));
    notifyProgress();
}

void RecoveryManager::notifyProgress() {
    if (progressCb_) progressCb_(received_, totalSize_, state_);
}

bool RecoveryManager::init(RecoveryChannel channel) {
    channel_ = channel;
    bleDfu_ = std::make_unique<BleDfu>();
    wifiOta_ = std::make_unique<WiFiOta>();
    uartDfu_ = std::make_unique<UartDfu>();

    auto handler = [this](const void* data, size_t len) {
        return receiveChunk(data, len);
    };

    bleDfu_->setDataHandler(handler);
    wifiOta_->setDataHandler(handler);
    uartDfu_->setDataHandler(handler);

    TAKT_LOGI(TAG, "Recovery init channel=%u", static_cast<uint8_t>(channel));
    return true;
}

bool RecoveryManager::startDfuListener() {
    setState(DfuState::AwaitingImage);
    switch (channel_) {
        case RecoveryChannel::Ble:  return bleDfu_->start();
        case RecoveryChannel::WiFi: return wifiOta_->start();
        case RecoveryChannel::Uart: return uartDfu_->start();
        default: return false;
    }
}

int RecoveryManager::receiveChunk(const void* data, size_t len) {
    if (state_ != DfuState::AwaitingImage && state_ != DfuState::Receiving) {
        return -1;
    }
    if (state_ == DfuState::AwaitingImage) {
        setState(DfuState::Receiving);
    }
    const int written = FirmwareCache::instance().writeChunk(data, len);
    if (written > 0) {
        received_ += static_cast<uint32_t>(written);
        EventBus::instance().publish(Event::OtaProgress, received_, totalSize_);
        notifyProgress();
    }
    return written;
}

bool RecoveryManager::finalizeDfu() {
    setState(DfuState::Verifying);
    if (!FirmwareCache::instance().finalizeWrite()) {
        setState(DfuState::Failed);
        EventBus::instance().publish(Event::OtaFailed);
        return false;
    }
    setState(DfuState::Installing);
    const uint8_t slot = FirmwareCache::instance().inactiveSlot();
    if (!FirmwareCache::instance().activateSlot(slot)) {
        setState(DfuState::Failed);
        return false;
    }
    setState(DfuState::Complete);
    EventBus::instance().publish(Event::OtaComplete);
    boot::Bootloader::requestOtaBoot(slot);
    TAKT_LOGI(TAG, "DFU complete, rebooting to slot %u", slot);
#ifdef TAKT_ESP32
    esp_restart();
#endif
    return true;
}

bool RecoveryManager::rollback() {
    setState(DfuState::RollingBack);
    if (!FirmwareCache::instance().rollback()) {
        setState(DfuState::Failed);
        return false;
    }
    setState(DfuState::Complete);
    EventBus::instance().publish(Event::OtaRollback);
    return true;
}

void RecoveryManager::abort() {
    setState(DfuState::Idle);
    received_ = 0;
    totalSize_ = 0;
    if (bleDfu_) bleDfu_->stop();
    if (wifiOta_) wifiOta_->stop();
}

void RecoveryManager::tick() {
    if (uartDfu_ && channel_ == RecoveryChannel::Uart) {
        uartDfu_->tick();
    }
}

void RecoveryManager::run() {
    TAKT_LOGI(TAG, "Recovery main loop");
    startDfuListener();
    while (state_ != DfuState::Complete && state_ != DfuState::Failed) {
        tick();
#ifdef TAKT_ESP32
        vTaskDelay(pdMS_TO_TICKS(10));
#endif
    }
}

} // namespace takt::recovery
