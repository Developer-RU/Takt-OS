// Copyright 2024-2026 Masyukov Pavel <p.masyukov@gmail.com>
// SPDX-License-Identifier: Apache-2.0
//
// TAKT OS — https://github.com/Developer-RU/Takt-OS

#include "takt/modules/mqtt_module.hpp"
#include "takt/event_bus.hpp"
#include "takt/nvs_manager.hpp"
#include "takt/logger.hpp"

#ifdef TAKT_ESP32
#include "mqtt_client.h"
#endif

namespace takt::modules {

namespace {
constexpr const char* TAG = "MQTT";

#ifdef TAKT_ESP32
esp_mqtt_client_handle_t gClient = nullptr;
MqttModule* gModule = nullptr;

void onMqttEvent(void* arg, esp_event_base_t base, int32_t eventId, void* data) {
    (void)base; (void)arg;
    if (!gModule) return;
    if (eventId == MQTT_EVENT_CONNECTED) {
        gModule->tick();
    } else if (eventId == MQTT_EVENT_DATA) {
        auto* event = static_cast<esp_mqtt_event_handle_t>(data);
        (void)event;
        gModule->tick();
    }
}
#endif
}

void MqttModule::setBroker(const char* uri) {
    std::strncpy(brokerUri_, uri, sizeof(brokerUri_) - 1);
}

bool MqttModule::init() {
    char uri[128]{};
    if (NvsManager::instance().getString("mqtt_broker", uri, sizeof(uri))) {
        setBroker(uri);
    }

#ifdef TAKT_ESP32
    gModule = this;
    esp_mqtt_client_config_t cfg = {};
    cfg.broker.address.uri = brokerUri_;
    gClient = esp_mqtt_client_init(&cfg);
    esp_mqtt_client_register_event(gClient, MQTT_EVENT_ANY, onMqttEvent, this);
    esp_mqtt_client_start(gClient);
#endif
    TAKT_LOGI(TAG, "MQTT init broker=%s", brokerUri_);
    return true;
}

void MqttModule::tick() {
#ifdef TAKT_ESP32
    if (gClient && esp_mqtt_client_get_outbox_size(gClient) >= 0) {
        if (!connected_) {
            connected_ = true;
            EventBus::instance().publish(Event::MqttConnected);
            TAKT_LOGI(TAG, "MQTT connected");
        }
    }
#endif
    if (msgPending_) {
        msgPending_ = false;
        if (pendingTopic_[0]) {
            publish(pendingTopic_, pendingPayload_);
            pendingTopic_[0] = '\0';
        }
    }
}

bool MqttModule::publish(const char* topic, const char* payload) {
    TAKT_LOGD(TAG, "Publish [%s]: %s", topic, payload);
#ifdef TAKT_ESP32
    if (!gClient || !connected_) {
        std::strncpy(pendingTopic_, topic, sizeof(pendingTopic_) - 1);
        std::strncpy(pendingPayload_, payload, sizeof(pendingPayload_) - 1);
        msgPending_ = true;
        return false;
    }
    return esp_mqtt_client_publish(gClient, topic, payload, 0, 1, 0) >= 0;
#else
    (void)topic; (void)payload;
    return connected_;
#endif
}

bool MqttModule::subscribe(const char* topic) {
#ifdef TAKT_ESP32
    if (!gClient) return false;
    return esp_mqtt_client_subscribe(gClient, topic, 1) >= 0;
#else
    (void)topic;
    return true;
#endif
}

void MqttModule::shutdown() {
#ifdef TAKT_ESP32
    if (gClient) {
        esp_mqtt_client_stop(gClient);
        esp_mqtt_client_destroy(gClient);
        gClient = nullptr;
    }
#endif
    connected_ = false;
    gModule = nullptr;
    TAKT_LOGI(TAG, "MQTT shutdown");
}

} // namespace takt::modules
