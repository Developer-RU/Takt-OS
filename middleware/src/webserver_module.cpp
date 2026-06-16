// Copyright 2024-2026 Masyukov Pavel <p.masyukov@gmail.com>
// SPDX-License-Identifier: Apache-2.0
//
// TAKT OS — https://github.com/Developer-RU/Takt-OS

#include "takt/modules/webserver_module.hpp"
#include "takt/kernel.hpp"
#include "takt/logger.hpp"
#include <cstdio>
#include <inttypes.h>

#ifdef TAKT_ESP32
#include "esp_http_server.h"
#endif

namespace takt::modules {

namespace {
constexpr const char* TAG = "WebServer";

#ifdef TAKT_ESP32
httpd_handle_t gServer = nullptr;

esp_err_t statusHandler(httpd_req_t* req) {
    httpd_resp_set_type(req, "application/json");
    httpd_resp_sendstr(req, "{\"status\":\"ok\",\"os\":\"TAKT OS\"}");
    return ESP_OK;
}

esp_err_t statsHandler(httpd_req_t* req) {
    auto stats = Kernel::instance().collectStats();
    char buf[256];
    std::snprintf(buf, sizeof(buf),
        "{\"uptime_ms\":%" PRIu32 ",\"takts\":%llu,\"heap_free\":%zu}",
        stats.uptimeMs,
        static_cast<unsigned long long>(stats.scheduler.totalTakts),
        stats.heapFree);
    httpd_resp_set_type(req, "application/json");
    httpd_resp_sendstr(req, buf);
    return ESP_OK;
}
#endif
}

bool WebServerModule::init() {
#ifdef TAKT_ESP32
    httpd_config_t config = HTTPD_DEFAULT_CONFIG();
    config.server_port = port_;
    if (httpd_start(&gServer, &config) != ESP_OK) return false;

    httpd_uri_t status = { .uri = "/api/status", .method = HTTP_GET, .handler = statusHandler };
    httpd_uri_t stats  = { .uri = "/api/stats",  .method = HTTP_GET, .handler = statsHandler };
    httpd_register_uri_handler(gServer, &status);
    httpd_register_uri_handler(gServer, &stats);
    running_ = true;
#endif
    TAKT_LOGI(TAG, "Web server on port %u", port_);
    return true;
}

void WebServerModule::tick() {}

void WebServerModule::shutdown() {
#ifdef TAKT_ESP32
    if (gServer) {
        httpd_stop(gServer);
        gServer = nullptr;
    }
#endif
    running_ = false;
    TAKT_LOGI(TAG, "Web server shutdown");
}

} // namespace takt::modules
