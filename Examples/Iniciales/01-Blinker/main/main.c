#include <stdio.h>
#include "driver/gpio.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "esp_log.h"
#include "sdkconfig.h"

#define DELAYBLINK 500

static void init_gpio(void)
{
    const gpio_config_t conf_gpio = {
        .pin_bit_mask = 1ULL << CONFIG_BLINK_GPIO,
        .intr_type = GPIO_INTR_DISABLE,
        .mode = GPIO_MODE_OUTPUT,
        .pull_down_en = GPIO_PULLDOWN_ENABLE,
        .pull_up_en = GPIO_PULLUP_DISABLE,
    };

    ESP_ERROR_CHECK(gpio_config(&conf_gpio));
}

static const char *TAG = "Ejemplo 01 - Blinker";

void app_main(void)
{
    init_gpio();

    while(1)
    {
        gpio_set_level(CONFIG_BLINK_GPIO, 1);
        ESP_LOGI(TAG, "Led encendido");

        vTaskDelay(pdMS_TO_TICKS(DELAYBLINK));

        gpio_set_level(CONFIG_BLINK_GPIO, 0);
        ESP_LOGI(TAG, "Led apagado");

        vTaskDelay(pdMS_TO_TICKS(DELAYBLINK));
    }

}
