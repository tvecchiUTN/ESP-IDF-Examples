#include <stdio.h>
#include "buttonIntr.h"
#include "freertos/semphr.h"
#include "esp_log.h"

static const char *TAG = "Ejemplo 02 - Button";


static void led_init()
{
    gpio_config_t conf_button = {};
    conf_button.intr_type = GPIO_INTR_DISABLE;
    conf_button.mode = GPIO_MODE_OUTPUT;
    conf_button.pin_bit_mask = 1ULL << 2;
    conf_button.pull_down_en = GPIO_PULLDOWN_ENABLE;
    conf_button.pull_up_en = GPIO_PULLUP_DISABLE;

    ESP_ERROR_CHECK(gpio_config(&conf_button));
}


void app_main(void)
{
    /*Polling
    button_init();
    while (1)
    {
        if (!gpio_get_level(PININPUT))
        {
            ESP_LOGD(TAG, "Boton presionado");
        }

        vTaskDelay(pdMS_TO_TICKS(1000));
    }
    */

    led_init();

    SemaphoreHandle_t manejador_sema = xSemaphoreCreateBinary();

    button_intr_init(manejador_sema);
    
    uint32_t state_pin = 0xFFFFFFFF;

    while (1)
    {
        if (xSemaphoreTake(manejador_sema, portMAX_DELAY))
        {
            ESP_LOGW(TAG, "Boton presionado");
            gpio_set_level(2, state_pin);
            state_pin = ~state_pin;
        }
    }
}
