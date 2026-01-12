#include <stdio.h>
#include "esp_log.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "driver/gpio.h"

#define STACK_SIZE 2048

#define LEDPIN 2
#define OFF 0
#define ON 1

void init_led(void)
{
    gpio_config_t led_config = {
        .mode = GPIO_MODE_OUTPUT,
        .intr_type = GPIO_INTR_DISABLE,
        .pin_bit_mask = 1ULL << LEDPIN,
        .pull_down_en = OFF,
        .pull_up_en = OFF
    };

    ESP_ERROR_CHECK(gpio_config(&led_config));
}

static const char *LOGPRINT = "PrintfTask";
void taskPrint(void* args)
{
    while(1)
    {
        ESP_LOGI(LOGPRINT, "Esto se esta ejecutando en paralelo cada 2000 ms\n");
        vTaskDelay(pdMS_TO_TICKS(2000));
    }
}

//static const char *LOGMAIN = "BlinkLed";
void app_main(void)
{
    xTaskCreate(taskPrint, LOGPRINT, STACK_SIZE, NULL, 1, NULL);

    init_led();
    while(1)
    {
        ESP_ERROR_CHECK(gpio_set_level(LEDPIN, ON));
        vTaskDelay(pdMS_TO_TICKS(700));
        ESP_ERROR_CHECK(gpio_set_level(LEDPIN, OFF));
        vTaskDelay(pdMS_TO_TICKS(700));
    }
}