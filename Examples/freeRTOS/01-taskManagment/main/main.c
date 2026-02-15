#include <stdio.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "freertos/event_groups.h"
#include "esp_err.h"
#include "esp_log.h"

#include "driver/gpio.h"

#define MAIN_FINISH_BIT 1 << 0

typedef struct
{
    gpio_num_t gpio_num;
    uint64_t blink_period_ms;
    EventGroupHandle_t handleGroup;
}blinker_led_t;

static const char* TAG = "Ejemplo 01-TaskManager";

void blink_task(void* pvParameters)
{
    blinker_led_t *blinker_data = (blinker_led_t*)pvParameters;

    gpio_config_t cfg_blinker = {
        .pin_bit_mask = 1ULL << blinker_data->gpio_num,
        .intr_type = GPIO_INTR_DISABLE,
        .mode = GPIO_MODE_OUTPUT,
        .pull_down_en = GPIO_PULLDOWN_ENABLE,
        .pull_up_en = GPIO_PULLDOWN_DISABLE,
    };

    ESP_ERROR_CHECK(gpio_config(&cfg_blinker));

    ESP_LOGI(TAG, "GPIO%d configurado correctamente en el nucleo %d. Su memoria utilizada en el stack es %u", blinker_data->gpio_num, xPortGetCoreID(), uxTaskGetStackHighWaterMark(NULL));

    //Lo seteamos a un nivel conocido
    gpio_set_level(blinker_data->gpio_num, 0);

    xEventGroupWaitBits(blinker_data->handleGroup, MAIN_FINISH_BIT, pdFALSE, pdTRUE, portMAX_DELAY);

    while(1)
    {
        gpio_set_level(blinker_data->gpio_num, 1);

        vTaskDelay(pdMS_TO_TICKS(blinker_data->blink_period_ms));

        gpio_set_level(blinker_data->gpio_num, 0);

        vTaskDelay(pdMS_TO_TICKS(blinker_data->blink_period_ms));
    }
}

/*
void cpu_kiler(void* args)
{
    while(1);
}
*/

void app_main(void)
{
    EventGroupHandle_t handleGroup_main = xEventGroupCreate();

    static blinker_led_t LED1 = {
        .gpio_num = GPIO_NUM_2,
        .blink_period_ms = 500,
    };
    LED1.handleGroup = handleGroup_main;

    static blinker_led_t LED2 = {
        .gpio_num = GPIO_NUM_4,
        .blink_period_ms = 1000,
    };
    LED2.handleGroup = handleGroup_main;

    TaskHandle_t handle_task_GPIO2;
    TaskHandle_t handle_task_GPIO4;

    static StackType_t stack_task_GPIO2[2048];
    static StaticTask_t tcb_task_GPIO2;

    handle_task_GPIO2 = xTaskCreateStaticPinnedToCore(blink_task, "Task GPIO2", 2048, (void*)&LED1, 1, stack_task_GPIO2, &tcb_task_GPIO2, 0);
    xTaskCreatePinnedToCore(blink_task, "Task GPIO4", 2048, (void*)&LED2, 2, &handle_task_GPIO4, 1);

    //Funcion para analizar el uso del watchdog
    //xTaskCreatePinnedToCore(cpu_kiler, "Task CPU killer", 512, NULL, 5, NULL, 0);

    if(handle_task_GPIO2 && handle_task_GPIO4)
    {
        ESP_LOGI(TAG, "Tareas creadas correctamente");
    }

    xEventGroupSetBits(handleGroup_main, MAIN_FINISH_BIT);

    vTaskSuspend(NULL);
}
