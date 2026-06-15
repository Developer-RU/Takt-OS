#include "takt/modules/wifi_module.hpp"
#include "takt/event_bus.hpp"
#include "takt/nvs_manager.hpp"
#include "takt/logger.hpp"

#ifdef TAKT_ESP32
#include "esp_wifi.h"
#include "esp_event.h"
#include "esp_netif.h"
#include "nvs_flash.h"
#endif

namespace takt::modules {

namespace {
constexpr const char* TAG = "WiFi";

#ifdef TAKT_ESP32
void onWifiEvent(void* arg, esp_event_base_t base, int32_t id, void* data) {
    auto* self = static_cast<WiFiModule*>(arg);
    if (base == WIFI_EVENT) {
        if (id == WIFI_EVENT_STA_DISCONNECTED) {
            self->tick(); // trigger reconnect via eventPending
        }
    } else if (base == IP_EVENT && id == IP_EVENT_STA_GOT_IP) {
        (void)data;
    }
}
#endif
}

void WiFiModule::setConfig(const WiFiConfig& cfg) {
    config_ = cfg;
}

bool WiFiModule::init() {
#ifdef TAKT_ESP32
    esp_netif_init();
    esp_event_loop_create_default();
    esp_netif_create_default_wifi_sta();

    wifi_init_config_t cfg = WIFI_INIT_CONFIG_DEFAULT();
    esp_wifi_init(&cfg);
    esp_event_handler_instance_register(WIFI_EVENT, ESP_EVENT_ANY_ID, onWifiEvent, this, nullptr);
    esp_event_handler_instance_register(IP_EVENT, IP_EVENT_STA_GOT_IP, onWifiEvent, this, nullptr);

    char ssid[32]{}, pass[64]{};
    if (NvsManager::instance().getString("wifi_ssid", ssid, sizeof(ssid))) {
        std::strncpy(config_.ssid, ssid, sizeof(config_.ssid) - 1);
    }
    if (NvsManager::instance().getString("wifi_pass", pass, sizeof(pass))) {
        std::strncpy(config_.password, pass, sizeof(config_.password) - 1);
    }

    wifi_config_t wcfg{};
    std::strncpy(reinterpret_cast<char*>(wcfg.sta.ssid), config_.ssid, sizeof(wcfg.sta.ssid));
    std::strncpy(reinterpret_cast<char*>(wcfg.sta.password), config_.password, sizeof(wcfg.sta.password));
    esp_wifi_set_mode(WIFI_MODE_STA);
    esp_wifi_set_config(WIFI_IF_STA, &wcfg);
    esp_wifi_start();
    initialized_ = true;
#endif

    reconnectTimer_.onTimeout([this]() {
        if (!connected_) reconnectPending_ = true;
    });
    reconnectTimer_.start();
    TAKT_LOGI(TAG, "WiFi init SSID=%s", config_.ssid);
    return true;
}

bool WiFiModule::connect() {
#ifdef TAKT_ESP32
    if (!initialized_) return false;
    const esp_err_t err = esp_wifi_connect();
    return err == ESP_OK;
#else
    connected_ = true;
    return true;
#endif
}

void WiFiModule::processEvents() {
    eventPending_ = false;
}

void WiFiModule::tick() {
    if (reconnectPending_) {
        reconnectPending_ = false;
        if (connect()) {
            eventPending_ = true;
        }
    }
#ifdef TAKT_ESP32
    wifi_ap_record_t ap{};
    if (esp_wifi_sta_get_ap_info(&ap) == ESP_OK) {
        if (!connected_) {
            connected_ = true;
            EventBus::instance().publish(Event::WiFiConnected);
            TAKT_LOGI(TAG, "WiFi connected to %s", reinterpret_cast<const char*>(ap.ssid));
        }
    }
#else
    if (!connected_) {
        connected_ = true;
        EventBus::instance().publish(Event::WiFiConnected);
    }
#endif
    if (eventPending_) processEvents();
}

void WiFiModule::shutdown() {
#ifdef TAKT_ESP32
    if (initialized_) esp_wifi_stop();
#endif
    connected_ = false;
    TAKT_LOGI(TAG, "WiFi shutdown");
}

} // namespace takt::modules
