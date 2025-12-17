#include <stdio.h>
#include "driver/dac_oneshot.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "esp_log.h"

static const char *LOG = "Ejemplo 4 - DAC";

void dac_config(dac_oneshot_handle_t *dac_handle)
{
    dac_oneshot_config_t dac_config = {};
    dac_config.chan_id = DAC_CHAN_0;

    ESP_ERROR_CHECK(dac_oneshot_new_channel(&dac_config, dac_handle));
}

void app_main(void)
{
    dac_oneshot_handle_t dac_handle;

    dac_config(&dac_handle);

    while (1)
    {
        ESP_LOGI(LOG, "Subiendo\n");
        int volt;
        for (volt = 0; volt < 255; volt += 5)
        {
            ESP_ERROR_CHECK(dac_oneshot_output_voltage(dac_handle, volt));
            vTaskDelay(pdMS_TO_TICKS(100));
        }

        ESP_LOGI(LOG, "Bajando\n");
        volt = 0;
        ESP_ERROR_CHECK(dac_oneshot_output_voltage(dac_handle, volt));
        vTaskDelay(pdMS_TO_TICKS(1000));
    }

    ESP_ERROR_CHECK(dac_oneshot_del_channel(dac_handle));
}