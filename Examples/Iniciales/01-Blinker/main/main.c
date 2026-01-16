#include "led_init.h"

#define DELAYBLINK 500

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
