#include "takt/kernel.hpp"
#include "takt/event_bus.hpp"
#include "takt/timer.hpp"
#include "takt/drivers/platform.hpp"
#include "takt/modules/uart_module.hpp"
#include "takt/modules/sensor_module.hpp"
#include "takt/modules/wifi_module.hpp"
#include "takt/modules/mqtt_module.hpp"
#include "takt/modules/ble_module.hpp"
#include "takt/modules/modbus_module.hpp"
#include "takt/modules/webserver_module.hpp"
#include "takt/modules/relay_module.hpp"
#include "takt/services/ota_service.hpp"
#include "takt/services/telemetry_service.hpp"
#include "takt/services/config_manager.hpp"
#include "takt/services/modbus_gateway.hpp"
#include "takt/bootloader.hpp"
#include "takt/logger.hpp"

static uint8_t gTelemetryPool[8192];

#ifdef TAKT_ESP32
extern "C" void app_main(void)
#else
int main()
#endif
{
    takt::drivers::Platform::init();

    auto& kernel = takt::Kernel::instance();
    auto& scheduler = kernel.scheduler();

    takt::modules::UartModule      uart(0, 16);
    takt::modules::SensorModule    sensor;
    takt::modules::RelayModule     relay(26);
    takt::modules::WiFiModule      wifi;
    takt::modules::MqttModule      mqtt;
    takt::modules::BleModule       ble;
    takt::modules::ModbusModule    modbus(takt::modules::ModbusModule::Mode::Rtu, 1);
    takt::modules::WebServerModule webserver;
    takt::services::OtaService     ota;
    takt::services::TelemetryService telemetry(gTelemetryPool, sizeof(gTelemetryPool));
    takt::services::ConfigManager  configMgr;
    takt::services::ModbusGateway  modbusGw;

    scheduler.registerModule(&uart);
    scheduler.registerModule(&sensor);
    scheduler.registerModule(&relay);
    scheduler.registerModule(&modbus);
    scheduler.registerModule(&wifi);
    scheduler.registerModule(&mqtt);
    scheduler.registerModule(&ble);
    scheduler.registerModule(&webserver);
    scheduler.registerModule(&ota);
    scheduler.registerModule(&telemetry);
    scheduler.registerModule(&configMgr);
    scheduler.registerModule(&modbusGw);

    modbusGw.setModbusModule(&modbus);
    webserver.setPort(80);

    scheduler.setTaktPeriodMs(1);
    scheduler.setTaktBudgetUs(8000);

    takt::EventBus::instance().subscribe(
        takt::Event::WiFiConnected,
        [](const takt::EventData&, void* ctx) {
            auto* m = static_cast<takt::modules::MqttModule*>(ctx);
            m->subscribe("takt/telemetry");
            TAKT_LOGI("DemoApp", "WiFi up — MQTT subscribed");
        }, &mqtt);

    takt::EventBus::instance().subscribe(
        takt::Event::SensorDataReady,
        [](const takt::EventData& data, void* ctx) {
            auto* tel = static_cast<takt::services::TelemetryService*>(ctx);
            takt::services::TelemetryRecord rec{};
            rec.timestamp = static_cast<uint32_t>(takt::drivers::Platform::getUptimeUs() / 1000000);
            rec.sensorValue = data.param1;
            tel->record(rec);
        }, &telemetry);

    takt::Timer statsTimer(30000, true);
    statsTimer.onTimeout([&kernel]() { kernel.printStatistics(); });
    statsTimer.start();

    takt::Timer demoTimer(10000, true);
    demoTimer.onTimeout([&relay]() {
        static bool on = false;
        on = !on;
        relay.setRelay(on);
    });
    demoTimer.start();

    if (!kernel.boot()) {
        TAKT_LOGE("DemoApp", "Kernel boot failed");
        return;
    }

    takt::boot::Bootloader::markBootSuccessful();
    kernel.run();

#ifndef TAKT_ESP32
    return 0;
#endif
}
