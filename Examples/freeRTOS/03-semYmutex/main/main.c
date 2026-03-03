#include <stdio.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "esp_log.h"
#include "freertos/semphr.h"

static const char *TAG = "Ejemplo 03 - Semaforos y Mutex";

void slow_task(void *args)
{
    SemaphoreHandle_t handle_mutex_slow = (SemaphoreHandle_t)args;

    BaseType_t mutex_avaible;
    while (1)
    {
        mutex_avaible = xSemaphoreTake(handle_mutex_slow, pdMS_TO_TICKS(100));
        if (mutex_avaible == pdTRUE)
        {
            for (int i = 0; i < 10; i++)
            {
                printf(".");
            }
            printf(" Terminado\n");
            xSemaphoreGive(handle_mutex_slow);
        }

        vTaskDelay(pdMS_TO_TICKS(500));
    }
}

void critical_task(void *args)
{
    SemaphoreHandle_t handle_mutex_critical = (SemaphoreHandle_t)args;

    BaseType_t mutex_avaible;
    while (1)
    {
        mutex_avaible = xSemaphoreTake(handle_mutex_critical, pdMS_TO_TICKS(500));
        if (mutex_avaible == pdTRUE)
        {
            ESP_LOGI("Tarea critica", " | CRITICO | ");
            xSemaphoreGive(handle_mutex_critical);
        }
        vTaskDelay(pdMS_TO_TICKS(100));
    }
}

void app_main(void)
{
    SemaphoreHandle_t handle_mutex = xSemaphoreCreateMutex();
    if (!handle_mutex)
    {
        ESP_LOGW(TAG, "No se pudo crear el mutex, hago algo");
    }

    xTaskCreate(slow_task, "Tarea lenta", 2048, (void *)handle_mutex, 1, NULL);
    xTaskCreate(critical_task, "Tarea critica", 2048, (void *)handle_mutex, 5, NULL);
}
