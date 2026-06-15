#include "takt/services/config_manager.hpp"
#include "takt/nvs_manager.hpp"
#include "takt/event_bus.hpp"
#include "takt/logger.hpp"
#include <cstring>

namespace takt::services {

namespace {
constexpr const char* TAG = "ConfigMgr";
}

bool ConfigManager::init() {
    TAKT_LOGI(TAG, "Config manager init");
    return true;
}

bool ConfigManager::setString(const char* key, const char* value) {
    if (!NvsManager::instance().setString(key, value)) return false;
    if (changeCb_) changeCb_(key);
    return true;
}

bool ConfigManager::getString(const char* key, char* buf, size_t maxLen) const {
    return NvsManager::instance().getString(key, buf, maxLen);
}

void ConfigManager::applyRemoteConfig(const char* json) {
    std::strncpy(pendingJson_, json, sizeof(pendingJson_) - 1);
    applyPending_ = true;
}

void ConfigManager::applyPending() {
    applyPending_ = false;
    // Minimal JSON parse: key=value pairs separated by ;
    char* ctx = nullptr;
    char* copy = pendingJson_;
    char* token = strtok_r(copy, ";", &ctx);
    while (token) {
        char* eq = std::strchr(token, '=');
        if (eq) {
            *eq = '\0';
            setString(token, eq + 1);
        }
        token = strtok_r(nullptr, ";", &ctx);
    }
    EventBus::instance().publish(Event::SystemReady);
    TAKT_LOGI(TAG, "Remote config applied");
}

void ConfigManager::tick() {
    if (applyPending_) applyPending();
}

void ConfigManager::shutdown() {
    NvsManager::instance().backup();
    TAKT_LOGI(TAG, "Config manager shutdown");
}

} // namespace takt::services
