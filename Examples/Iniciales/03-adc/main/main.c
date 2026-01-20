#include <stdio.h>
#include "esp_adc/adc_oneshot.h"
#include "esp_adc/adc_cali.h"
#include "esp_adc/adc_cali_scheme.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "esp_log.h"

#define CHAN_ADC ADC_CHANNEL_4
#define UNIT_ADC ADC_UNIT_1
#define ATTEN_ADC ADC_ATTEN_DB_12
#define BITW_ADC ADC_BITWIDTH_DEFAULT

static const char *TAG = "Ejemplo 03 - adc";

/*
static void init_adc_raw(adc_oneshot_unit_handle_t *handle_adc)
{
    adc_oneshot_unit_init_cfg_t cfg_adc_oneshot = {0};
    cfg_adc_oneshot.clk_src = 0;
    cfg_adc_oneshot.ulp_mode = ADC_ULP_MODE_DISABLE;
    cfg_adc_oneshot.unit_id = UNIT_ADC;

    ESP_ERROR_CHECK(adc_oneshot_new_unit(&cfg_adc_oneshot, handle_adc));

    adc_oneshot_chan_cfg_t cfg_adc_oneshot_chan = {0};
    cfg_adc_oneshot_chan.atten = ATTEN_ADC;
    cfg_adc_oneshot_chan.bitwidth = BITW_ADC;

    ESP_ERROR_CHECK(adc_oneshot_config_channel(*handle_adc, CHAN_ADC, &cfg_adc_oneshot_chan));
}
*/

static void init_adc_cali(adc_oneshot_unit_handle_t *handle_adc, adc_cali_handle_t *hande_cali)
{
    adc_oneshot_unit_init_cfg_t cfg_adc_oneshot = {0};
    cfg_adc_oneshot.clk_src = 0;
    cfg_adc_oneshot.ulp_mode = ADC_ULP_MODE_DISABLE;
    cfg_adc_oneshot.unit_id = UNIT_ADC;

    ESP_ERROR_CHECK(adc_oneshot_new_unit(&cfg_adc_oneshot, handle_adc));

    adc_oneshot_chan_cfg_t cfg_adc_oneshot_chan = {0};
    cfg_adc_oneshot_chan.atten = ATTEN_ADC;
    cfg_adc_oneshot_chan.bitwidth = BITW_ADC;

    ESP_ERROR_CHECK(adc_oneshot_config_channel(*handle_adc, CHAN_ADC, &cfg_adc_oneshot_chan));

    adc_cali_line_fitting_config_t cfg_cali_line = {0};
    cfg_cali_line.atten = ATTEN_ADC;
    cfg_cali_line.bitwidth = BITW_ADC;
    cfg_cali_line.default_vref = 0;
    cfg_cali_line.unit_id = UNIT_ADC;

    ESP_ERROR_CHECK(adc_cali_create_scheme_line_fitting(&cfg_cali_line, hande_cali));
}

void app_main(void)
{
    adc_oneshot_unit_handle_t handle_adc = NULL;
    adc_cali_handle_t handle_cali = NULL;
    // init_adc_raw(&handle_adc);
    init_adc_cali(&handle_adc, &handle_cali);

    int raw_adc;
    int volt;
    while (1)
    {
        uint32_t promedio = 0;
        /*Valor crudo
        esp_err_t err_raw = adc_oneshot_read(handle_adc, CHAN_ADC, &raw_adc);
        if(err_raw != ESP_OK)
        {
            ESP_LOGW(TAG, "Error leyendo ADC: %s", esp_err_to_name(err_raw));
            continue;
        }

        ESP_LOGI(TAG, "El valor del adc es %d", raw_adc);
        */

        for (int i = 0; i < 64; i++)
        {
            esp_err_t err_raw = adc_oneshot_read(handle_adc, CHAN_ADC, &raw_adc);
            if (err_raw != ESP_OK)
            {
                ESP_LOGW(TAG, "Error leyendo ADC: %s", esp_err_to_name(err_raw));
                break;
            }

            promedio += raw_adc;
        }

        promedio = promedio / 64;

        esp_err_t err_cali = adc_cali_raw_to_voltage(handle_cali, promedio, &volt);
        if (err_cali != ESP_OK)
        {
            ESP_LOGW(TAG, "Error leyendo ADC: %s", esp_err_to_name(err_cali));
            continue;
        }

        ESP_LOGI(TAG, "El valor en mV es %d", volt);

        vTaskDelay(pdMS_TO_TICKS(1000));
    }
}
