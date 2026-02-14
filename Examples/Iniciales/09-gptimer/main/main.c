#include <stdio.h>

#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "esp_log.h"

#include "driver/gptimer.h"

#define GPTIMER_FREC 1000 * 1000

static const char* TAG = "Ejemplo 09 - GPTimer";

void app_main(void)
{
    gptimer_config_t cfg_gptimer = {
        .clk_src = GPTIMER_CLK_SRC_DEFAULT,
        .direction = GPTIMER_COUNT_UP,
        .resolution_hz = GPTIMER_FREC,
    };

    gptimer_handle_t handle_gptimer = NULL;

    ESP_ERROR_CHECK(gptimer_new_timer(&cfg_gptimer, &handle_gptimer));

    ESP_ERROR_CHECK(gptimer_enable(handle_gptimer));

    ESP_ERROR_CHECK(gptimer_set_raw_count(handle_gptimer, 0));

    ESP_ERROR_CHECK(gptimer_start(handle_gptimer));

    uint64_t timer_value;
    double time;
    uint32_t resolution_hz;

    ESP_LOGI(TAG, "Timer seteado correctamente");

    while(1)
    {
        //Medicion mediante polling
        ESP_ERROR_CHECK(gptimer_get_raw_count(handle_gptimer, &timer_value));
        ESP_ERROR_CHECK(gptimer_get_resolution(handle_gptimer, &resolution_hz));
        time = (double)timer_value / resolution_hz;

        if(time > 10.0)
        {
            ESP_LOGI(TAG, "El cronometro supero los 10 segundos");
            gptimer_stop(handle_gptimer);
        }

        vTaskDelay(pdMS_TO_TICKS(1000));
    }
}
