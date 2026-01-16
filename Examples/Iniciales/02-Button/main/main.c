#include <stdio.h>
#include "driver/gpio.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "esp_log.h"

#define PININPUT 0

volatile uint8_t flagState = 0;

static const char* TAG = "Ejemplo 02 - Button";

void IRAM_ATTR pin_intr_func(void* args)
{
    flagState = 1;
}

static void button_init()
{
    gpio_config_t conf_button = {};
    conf_button.intr_type = GPIO_INTR_POSEDGE; //When it goes from 0 to 1, it raises
    conf_button.mode = GPIO_MODE_INPUT;
    conf_button.pin_bit_mask = 1ULL << PININPUT;
    conf_button.pull_down_en = GPIO_PULLDOWN_DISABLE;
    conf_button.pull_up_en = GPIO_PULLUP_ENABLE;

    ESP_ERROR_CHECK(gpio_config(&conf_button));

    //Register of the interrupt mode

    ESP_ERROR_CHECK(gpio_install_isr_service(0));

    /**
    In edge situations, you have to chech if the functions works. It will use heap memory so it could fails if theres no memory. 
    In that case, you may have to cancel the interrupt service
    */

    //gpio_isr_handle_t button_handle = NULL;
    //ESP_ERROR_CHECK(gpio_isr_register(pin_intr_func, NULL, 0, button_handle));

    ESP_ERROR_CHECK(gpio_isr_handler_add(PININPUT, pin_intr_func, NULL));
}

void app_main(void)
{
    button_init();

    while(1)
    {
        vTaskDelay(pdMS_TO_TICKS(1000));
        if(flagState)
        {
            flagState = 0;
            ESP_LOGI(TAG, "Boton presionado");
        }
    }

}
