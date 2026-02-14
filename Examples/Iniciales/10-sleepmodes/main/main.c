#include <stdio.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "esp_log.h"
#include "esp_sleep.h"
#include "driver/rtc_io.h"
#include "driver/uart.h"

static const char* TAG = "Ejemplo 10 - Sleep modes";

void app_main(void)
{
    esp_sleep_enable_timer_wakeup(5000000);

    uint32_t contador_dormido = 0;

    esp_err_t err_sleep;
    while(1)
    {
        ESP_LOGI(TAG, "Chau, me voy a dormir, ahora el contador es %u", contador_dormido);
        
        //Espero a que se envien todos los datos
        ESP_ERROR_CHECK(uart_wait_tx_idle_polling(CONFIG_ESP_CONSOLE_UART_NUM));

        err_sleep = esp_light_sleep_start();
        ESP_ERROR_CHECK(err_sleep);

        esp_sleep_wakeup_cause_t wakeup_src = esp_sleep_get_wakeup_cause();

        if(wakeup_src == ESP_SLEEP_WAKEUP_TIMER)
        {
            ESP_LOGI(TAG, "Me levante por el timer, que bien");
        }
        else if(wakeup_src != ESP_SLEEP_WAKEUP_TIMER)
        {
            ESP_LOGI(TAG, "Me levante por otro motivo, su codigo es %d", wakeup_src);
        }

        contador_dormido++;

        //vTaskDelay(pdMS_TO_TICKS(300));
    }
}
