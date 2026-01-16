#include "led_init.h"

esp_err_t init_gpio(void)
{
    const gpio_config_t conf_gpio = {
        .pin_bit_mask = 1ULL << CONFIG_BLINK_GPIO,
        .intr_type = GPIO_INTR_DISABLE,
        .mode = GPIO_MODE_OUTPUT,
        .pull_down_en = GPIO_PULLDOWN_ENABLE,
        .pull_up_en = GPIO_PULLUP_DISABLE,
    };

    ESP_ERROR_CHECK(gpio_config(&conf_gpio));

    return ESP_OK;
}