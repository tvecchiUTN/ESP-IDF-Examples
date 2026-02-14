#include <stdio.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "esp_log.h"
#include "esp_sleep.h"
#include "driver/rtc_io.h"
#include "driver/uart.h"

#include "driver/gpio.h"

static const char *TAG = "Ejemplo 10 - Sleep modes";

void main_light_sleep(void)
{
    esp_sleep_enable_timer_wakeup(5000000);

    uint32_t contador_dormido = 0;

    esp_err_t err_sleep;
    while (1)
    {
        ESP_LOGI(TAG, "Chau, me voy a dormir, ahora el contador es %u", contador_dormido);

        // Espero a que se envien todos los datos
        ESP_ERROR_CHECK(uart_wait_tx_idle_polling(CONFIG_ESP_CONSOLE_UART_NUM));

        err_sleep = esp_light_sleep_start();
        ESP_ERROR_CHECK(err_sleep);

        esp_sleep_wakeup_cause_t wakeup_src = esp_sleep_get_wakeup_cause();

        if (wakeup_src == ESP_SLEEP_WAKEUP_TIMER)
        {
            ESP_LOGI(TAG, "Me levante por el timer, que bien");
        }
        else if (wakeup_src != ESP_SLEEP_WAKEUP_TIMER)
        {
            ESP_LOGI(TAG, "Me levante por otro motivo, su codigo es %d", wakeup_src);
        }

        contador_dormido++;

        // vTaskDelay(pdMS_TO_TICKS(300));
    }
}

RTC_DATA_ATTR static uint32_t contador_profundo = 0;

void main_deep_sleep_timer()
{
    esp_sleep_wakeup_cause_t wakeup_src = esp_sleep_get_wakeup_cause();

    if (wakeup_src == ESP_SLEEP_WAKEUP_TIMER)
    {
        ESP_LOGI(TAG, "Me levante por el timer, que bien");
    }
    else if (wakeup_src != ESP_SLEEP_WAKEUP_TIMER)
    {
        ESP_LOGI(TAG, "Me levante por otro motivo, su codigo es %d", wakeup_src);
    }

    esp_sleep_enable_timer_wakeup(5000000);

    ESP_LOGI(TAG, "Chau, me voy a dormir, ahora el contador es %u", contador_profundo);

    contador_profundo++;

    // Espero a que se envien todos los datos
    ESP_ERROR_CHECK(uart_wait_tx_idle_polling(CONFIG_ESP_CONSOLE_UART_NUM));

    esp_deep_sleep_start();
}

void main_deep_sleep_pin()
{
    esp_sleep_wakeup_cause_t wakeup_src = esp_sleep_get_wakeup_cause();

    if (wakeup_src == ESP_SLEEP_WAKEUP_EXT0)
    {
        ESP_LOGI(TAG, "Me levante por un pulsador");
    }
    else if (wakeup_src != ESP_SLEEP_WAKEUP_EXT0)
    {
        ESP_LOGI(TAG, "Me levante por otro motivo, su codigo es %d", wakeup_src);
    }

    esp_sleep_enable_ext0_wakeup(GPIO_NUM_0, 0);

    rtc_gpio_pullup_en(GPIO_NUM_0);

    ESP_LOGI(TAG, "Chau, me voy a dormir, ahora el contador es %u", contador_profundo);

    contador_profundo++;

    ESP_ERROR_CHECK(uart_wait_tx_idle_polling(CONFIG_ESP_CONSOLE_UART_NUM));

    esp_deep_sleep_start();
}

void app_main()
{
    gpio_config_t cfg_gpio_2 = {
        .pin_bit_mask = 1ULL << GPIO_NUM_2,
        .intr_type = GPIO_INTR_DISABLE,
        .mode = GPIO_MODE_OUTPUT,
        .pull_down_en = GPIO_PULLDOWN_ENABLE,
        .pull_up_en = GPIO_PULLUP_DISABLE,
    };

    ESP_ERROR_CHECK(gpio_config(&cfg_gpio_2));

    ESP_ERROR_CHECK(rtc_gpio_hold_dis(GPIO_NUM_2));

    gpio_set_level(GPIO_NUM_2, 1);

    ESP_ERROR_CHECK(rtc_gpio_hold_en(GPIO_NUM_2));

    esp_sleep_wakeup_cause_t wakeup_src = esp_sleep_get_wakeup_cause();

    if (wakeup_src == ESP_SLEEP_WAKEUP_EXT0)
    {
        ESP_LOGI(TAG, "Me levante por un pulsador");
    }
    else if (wakeup_src != ESP_SLEEP_WAKEUP_EXT0)
    {
        ESP_LOGI(TAG, "Me levante por otro motivo, su codigo es %d", wakeup_src);
    }

    esp_sleep_enable_ext0_wakeup(GPIO_NUM_0, 0);

    rtc_gpio_pullup_en(GPIO_NUM_0);

    ESP_LOGI(TAG, "Chau, me voy a dormir, ahora el contador es %u", contador_profundo);

    contador_profundo++;

    ESP_ERROR_CHECK(uart_wait_tx_idle_polling(CONFIG_ESP_CONSOLE_UART_NUM));

    esp_deep_sleep_start();
}