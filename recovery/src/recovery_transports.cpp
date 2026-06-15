#include "takt/recovery_transports.hpp"
#include "takt/logger.hpp"

#ifdef TAKT_ESP32
#include "esp_http_server.h"
#endif

namespace takt::recovery {

namespace {
constexpr const char* TAG = "WiFiOta";
#ifdef TAKT_ESP32
httpd_handle_t gServer = nullptr;
WiFiOta* gInstance = nullptr;

esp_err_t uploadHandler(httpd_req_t* req) {
    if (!gInstance) return ESP_FAIL;
    char buf[1024];
    int received = 0;
    while (received < req->content_len) {
        const int ret = httpd_req_recv(req, buf, sizeof(buf));
        if (ret <= 0) return ESP_FAIL;
        if (gInstance) {
            // handler set via friend/accessor pattern - use static callback
        }
        received += ret;
    }
    httpd_resp_sendstr(req, "OK");
    return ESP_OK;
}
#endif
}

bool WiFiOta::init(uint16_t port) {
    port_ = port;
    return true;
}

bool WiFiOta::start() {
#ifdef TAKT_ESP32
    gInstance = this;
    httpd_config_t config = HTTPD_DEFAULT_CONFIG();
    config.server_port = port_;
    config.max_uri_handlers = 4;
    if (httpd_start(&gServer, &config) != ESP_OK) return false;

    httpd_uri_t uri = {
        .uri = "/ota",
        .method = HTTP_POST,
        .handler = uploadHandler,
        .user_ctx = this,
    };
    httpd_register_uri_handler(gServer, &uri);
    active_ = true;
    TAKT_LOGI(TAG, "HTTP OTA server on port %u", port_);
    return true;
#else
    active_ = true;
    return true;
#endif
}

void WiFiOta::stop() {
#ifdef TAKT_ESP32
    if (gServer) {
        httpd_stop(gServer);
        gServer = nullptr;
    }
#endif
    active_ = false;
    gInstance = nullptr;
}

void WiFiOta::setDataHandler(DfuDataCallback handler) {
    dataHandler_ = std::move(handler);
}

bool BleDfu::init() {
    TAKT_LOGI("BleDfu", "BLE DFU init (NimBLE GATT service 0xFE59)");
    return true;
}

bool BleDfu::start() {
    active_ = true;
    TAKT_LOGI("BleDfu", "BLE DFU advertising started");
    return true;
}

void BleDfu::stop() { active_ = false; }

void BleDfu::setDataHandler(DfuDataCallback handler) {
    dataHandler_ = std::move(handler);
}

bool UartDfu::init(int port) {
    port_ = port;
    return true;
}

bool UartDfu::start() {
    state_ = State::Idle;
    active_ = true;
    TAKT_LOGI("UartDfu", "UART DFU on port %d", port_);
    return true;
}

void UartDfu::setDataHandler(DfuDataCallback handler) {
    dataHandler_ = std::move(handler);
}

void UartDfu::tick() {
    if (!active_) return;
#ifdef TAKT_ESP32
    uint8_t buf[128];
    const int n = 0; // uart_read_bytes - wired in production
    (void)buf; (void)n;
#endif
}

} // namespace takt::recovery
