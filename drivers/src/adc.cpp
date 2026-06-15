#include "takt/drivers/platform.hpp"
#include "takt/logger.hpp"

#ifdef TAKT_ESP32
#include "esp_adc/adc_oneshot.h"
#include "esp_adc/adc_cali.h"
#include "esp_adc/adc_cali_scheme.h"
#endif

namespace takt::drivers {

namespace {
constexpr const char* TAG = "Adc";
#ifdef TAKT_ESP32
adc_oneshot_unit_handle_t adcHandle = nullptr;
adc_cali_handle_t caliHandle = nullptr;
bool caliEnabled = false;
#endif
}

bool Adc::init(int channel) {
#ifdef TAKT_ESP32
    (void)channel;
    if (adcHandle) return true;

    adc_oneshot_unit_init_cfg_t initCfg = {
        .unit_id = ADC_UNIT_1,
        .clk_src = ADC_RTC_CLK_SRC_DEFAULT,
        .ulp_mode = ADC_ULP_MODE_DISABLE,
    };
    if (adc_oneshot_new_unit(&initCfg, &adcHandle) != ESP_OK) return false;

    adc_oneshot_chan_cfg_t chanCfg = {
        .atten = ADC_ATTEN_DB_12,
        .bitwidth = ADC_BITWIDTH_DEFAULT,
    };
    adc_oneshot_config_channel(adcHandle, ADC_CHANNEL_0, &chanCfg);

#if ADC_CALI_SCHEME_LINE_FITTING_SUPPORTED
    adc_cali_line_fitting_config_t caliCfg = {
        .unit_id = ADC_UNIT_1,
        .atten = ADC_ATTEN_DB_12,
        .bitwidth = ADC_BITWIDTH_DEFAULT,
    };
    caliEnabled = (adc_cali_create_scheme_line_fitting(&caliCfg, &caliHandle) == ESP_OK);
#else
    caliEnabled = false;
#endif
    TAKT_LOGI(TAG, "ADC init, calibration=%s", caliEnabled ? "yes" : "no");
    return true;
#else
    (void)channel;
    return true;
#endif
}

int Adc::readRaw(int channel) {
#ifdef TAKT_ESP32
    if (!adcHandle) return -1;
    int raw = 0;
    const adc_channel_t ch = static_cast<adc_channel_t>(channel);
    if (adc_oneshot_read(adcHandle, ch, &raw) != ESP_OK) return -1;
    return raw;
#else
    (void)channel;
    return 2048;
#endif
}

float Adc::readVoltage(int channel) {
#ifdef TAKT_ESP32
    const int raw = readRaw(channel);
    if (raw < 0) return -1.0f;
    if (caliEnabled && caliHandle) {
        int mv = 0;
        if (adc_cali_raw_to_voltage(caliHandle, raw, &mv) == ESP_OK) {
            return static_cast<float>(mv) / 1000.0f;
        }
    }
    return static_cast<float>(raw) * 3.3f / 4095.0f;
#else
    (void)channel;
    return 1.65f;
#endif
}

} // namespace takt::drivers
