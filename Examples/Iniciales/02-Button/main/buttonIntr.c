#include "buttonIntr.h"

void IRAM_ATTR func_intr(void *args)
{
    SemaphoreHandle_t main_handle = (SemaphoreHandle_t)args;

    BaseType_t priority_main = pdFALSE;

    xSemaphoreGiveFromISR(main_handle, &priority_main);

    if (priority_main)
    {
        portYIELD_FROM_ISR();
    }
}

void button_intr_init(const SemaphoreHandle_t SemaHandle)
{
    gpio_config_t conf_button = {};
    conf_button.intr_type = GPIO_INTR_NEGEDGE; // Al presionar obtengo 0
    conf_button.mode = GPIO_MODE_INPUT;
    conf_button.pin_bit_mask = 1ULL << PININPUT;
    conf_button.pull_down_en = GPIO_PULLDOWN_DISABLE;
    conf_button.pull_up_en = GPIO_PULLUP_ENABLE;

    ESP_ERROR_CHECK(gpio_config(&conf_button));

    // Instalamos el servicio ISR

    ESP_ERROR_CHECK(gpio_install_isr_service(0));

    // Manejador del ISR

    ESP_ERROR_CHECK(gpio_isr_handler_add(PININPUT, func_intr, (void *)SemaHandle));
}