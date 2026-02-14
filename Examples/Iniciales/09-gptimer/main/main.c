#include <stdio.h>

#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "esp_log.h"

#include "driver/gptimer.h"

#define GPTIMER_FREC 1000 * 1000

static IRAM_ATTR bool func_alarm_gptimer(gptimer_handle_t timer, const gptimer_alarm_event_data_t *edata, void *user_ctx)
{
    BaseType_t high_task_awoken = pdFALSE;
    TaskHandle_t task_to_notify = (TaskHandle_t)user_ctx;

    vTaskNotifyGiveFromISR(task_to_notify, &high_task_awoken);

    return high_task_awoken == pdTRUE;
}

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

    gptimer_alarm_config_t cfg_alarm_gptimer = {
        .alarm_count = 10000000,
        .reload_count = 0,
        .flags.auto_reload_on_alarm = true,
    };

    ESP_ERROR_CHECK(gptimer_set_alarm_action(handle_gptimer, &cfg_alarm_gptimer));

    gptimer_event_callbacks_t cb_funcs = {
        .on_alarm = func_alarm_gptimer,
    };

    TaskHandle_t mainTask = xTaskGetCurrentTaskHandle();

    ESP_ERROR_CHECK(gptimer_register_event_callbacks(handle_gptimer, &cb_funcs, (void*)mainTask));

    ESP_ERROR_CHECK(gptimer_enable(handle_gptimer));

    ESP_ERROR_CHECK(gptimer_set_raw_count(handle_gptimer, 0));

    ESP_ERROR_CHECK(gptimer_start(handle_gptimer));

    ESP_LOGI(TAG, "Timer seteado correctamente");

    while(1)
    {
        /*
        //Medicion mediante polling
        ESP_ERROR_CHECK(gptimer_get_raw_count(handle_gptimer, &timer_value));
        ESP_ERROR_CHECK(gptimer_get_resolution(handle_gptimer, &resolution_hz));
        time = (double)timer_value / resolution_hz;

        if(time > 10.0)
        {
            ESP_LOGI(TAG, "El cronometro supero los 10 segundos");
            gptimer_stop(handle_gptimer);
        }
        */

        //Medicion por interrupcion
        if(ulTaskNotifyTake(pdTRUE, portMAX_DELAY))
        {
            ESP_LOGI(TAG, "Me levante gracias a la alarma");
        }
    }
}
