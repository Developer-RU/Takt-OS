#include "takt/recovery_manager.hpp"
#include "takt/drivers/platform.hpp"
#include "takt/logger.hpp"

extern "C" void app_main(void) {
    takt::drivers::Platform::init();
    takt::TAKT_LOGI("RecoveryApp", "TAKT OS Recovery v0.2.0");

    auto& recovery = takt::recovery::RecoveryManager::instance();
    recovery.init(takt::recovery::RecoveryChannel::WiFi);
    recovery.onProgress([](uint32_t rx, uint32_t total, auto state) {
        takt::TAKT_LOGI("RecoveryApp", "DFU %u/%u state=%u",
                        rx, total, static_cast<uint8_t>(state));
    });
    recovery.run();
}
