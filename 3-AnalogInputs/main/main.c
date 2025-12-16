#include <stdio.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "driver/gpio.h"
#include "esp_adc/adc_oneshot.h"
#include "esp_log.h"

static const char *TAG = "Ejemplo 3 entrada analoga";

void init_analog_in(adc_oneshot_unit_handle_t *pot_handle)
{
    adc_oneshot_unit_init_cfg_t pot_conf = {};
    pot_conf.unit_id = ADC_UNIT_1;
    pot_conf.ulp_mode = ADC_ULP_MODE_DISABLE;
    pot_conf.clk_src = ADC_RTC_CLK_SRC_DEFAULT;

    ESP_ERROR_CHECK(adc_oneshot_new_unit(&pot_conf, pot_handle));

    adc_oneshot_chan_cfg_t pot_chan = {};
    pot_chan.atten = ADC_ATTEN_DB_12;
    pot_chan.bitwidth = ADC_BITWIDTH_DEFAULT;

    ESP_ERROR_CHECK(adc_oneshot_config_channel(*pot_handle, ADC_CHANNEL_6, &pot_chan));

    ESP_LOGI(TAG, "ADC Inicializado correctamente");

}

void app_main(void)
{
    adc_oneshot_unit_handle_t pot_handle;

    init_analog_in(&pot_handle);

    while(1)
    {
        int entrada = 0;

        ESP_ERROR_CHECK(adc_oneshot_read(pot_handle, ADC_CHANNEL_6, &entrada));

        printf("%d\n", entrada);

        //ESP_LOGI(TAG, "Señal detectada");}
        vTaskDelay(pdMS_TO_TICKS(100));
    }

    adc_oneshot_del_unit(pot_handle);
}