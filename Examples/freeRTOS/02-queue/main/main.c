#include <stdio.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "freertos/queue.h"
#include "esp_log.h"
#include "esp_err.h"
#include "driver/gpio.h"

#include "esp_random.h"

//static const char* TAG = "Ejemplo 02 - QUEUE";

typedef enum data_source_t
{
    SOURCE_SENSOR_A, //Sensor de temperatura
    SOURCE_SENSOR_B, //Sensor de humedad
    SOURCE_EMERGENCY_BUTTON, //Boton de emergencia
}data_source_t;

typedef struct
{
    data_source_t source_id;
    uint32_t data_sensor;
}event_msg_t;

typedef struct
{
    data_source_t source_id;
    QueueHandle_t handle_queue;
}args_task_t;

static const char* TAG = "Salida main";

uint32_t esp_rand_limit(int min, int max)
{
    if(min > max)
    {
        return 0;
    }
    return (esp_random() % (max - min + 1)) + min;
}

void productor_task(void* pdArg)
{
    args_task_t *param_task = (args_task_t*)pdArg;

    BaseType_t queue_ret;

    event_msg_t msg_send_queue;

    while(1)
    {
        if(param_task->source_id == SOURCE_SENSOR_A)
        {
            msg_send_queue.source_id = SOURCE_SENSOR_A;
            msg_send_queue.data_sensor = esp_rand_limit(253, 373);
        }else if(param_task->source_id == SOURCE_SENSOR_B)
        {
            msg_send_queue.source_id = SOURCE_SENSOR_B;
            msg_send_queue.data_sensor = esp_rand_limit(0, 100);
        }

        queue_ret = xQueueSend(param_task->handle_queue, &msg_send_queue, pdMS_TO_TICKS(100));

        if(queue_ret == errQUEUE_FULL)
        {
            ESP_LOGE("Tarea productor", "Error al enviar por cola, espacio insuficiente");
        }
        else if(queue_ret != pdTRUE)
        {
            ESP_LOGE("Tarea productor", "Error al enviar por cola, otro error");
        }

        if(param_task->source_id == SOURCE_SENSOR_A)
        {
            vTaskDelay(pdMS_TO_TICKS(500));
        }
        else if(param_task->source_id == SOURCE_SENSOR_B)
        {
            vTaskDelay(pdMS_TO_TICKS(1000));
        }
    }
}

void consumer_task(void* pdArg)
{
    QueueHandle_t handle_queue_task = (QueueHandle_t)pdArg;

    while(1)
    {
        event_msg_t msg_queue_rcv;

        xQueueReceive(handle_queue_task, (event_msg_t*)&msg_queue_rcv, portMAX_DELAY);

        switch(msg_queue_rcv.source_id)
        {
            case SOURCE_SENSOR_A:
                ESP_LOGI("Tarea consumidor", "La temperatura es %d", msg_queue_rcv.data_sensor-273);
            break;

            case SOURCE_SENSOR_B:
                ESP_LOGI("Tarea consumidor", "La humedad es de %u", msg_queue_rcv.data_sensor);
            break;

            case SOURCE_EMERGENCY_BUTTON:
                ESP_LOGI("Tarea consumidor", "Boton presionado, cancelando salida de misiles");
            break;

            default:
                ESP_LOGI("Tarea consumidor", "No se pudo identificar de donde provino el mensaje");
            break;
        }
    }
}

void IRAM_ATTR intr_emergency_button(void* args)
{
    args_task_t *args_intr_button = (args_task_t*)args;

    event_msg_t msg_send_intr = 
    {
        .source_id = args_intr_button->source_id,
        .data_sensor = 1,
    };


    BaseType_t pdTaskWoken;
    xQueueSendFromISR(args_intr_button->handle_queue, (void*)&msg_send_intr, &pdTaskWoken);

    if(pdTaskWoken != pdTRUE)
    {
        portYIELD_FROM_ISR();
    }
}

void app_main(void)
{
    QueueHandle_t handle_queue_main = xQueueCreate(10, sizeof(event_msg_t));

    static args_task_t args_task_temp;
    args_task_temp.source_id = SOURCE_SENSOR_A;
    args_task_temp.handle_queue = handle_queue_main;

    static args_task_t args_task_hum;
    args_task_hum.source_id = SOURCE_SENSOR_B;
    args_task_hum.handle_queue = handle_queue_main;

    static args_task_t args_intr_button;
    args_intr_button.source_id = SOURCE_EMERGENCY_BUTTON;
    args_intr_button.handle_queue = handle_queue_main;

    gpio_config_t cfg_button = {
        .intr_type = GPIO_INTR_NEGEDGE,
        .mode = GPIO_MODE_INPUT,
        .pin_bit_mask = 1ULL << GPIO_NUM_0,
        .pull_up_en = GPIO_PULLUP_ENABLE,
        .pull_down_en = GPIO_PULLDOWN_DISABLE,
    };

    esp_err_t err_intr_button = gpio_config(&cfg_button);
    if(err_intr_button != ESP_OK)
    {
        ESP_LOGW(TAG, "No se pudo iniciar el gpio0");
    }

    err_intr_button = gpio_install_isr_service(0);
    if(err_intr_button != ESP_OK)
    {
        ESP_LOGW(TAG, "Error al iniciar el modo interrupcion");
    }
    else { ESP_ERROR_CHECK(gpio_isr_handler_add(GPIO_NUM_0, intr_emergency_button, &args_intr_button)); }

    xTaskCreate(productor_task, "Sensor temp", 2048, (void*)&args_task_temp, 1, NULL);
    xTaskCreate(productor_task, "Sensor hum", 2048, (void*)&args_task_hum, 1, NULL);

    xTaskCreate(consumer_task, "Consumer", 2048, (void*)handle_queue_main, 3, NULL);

    printf("Minimum free heap size: %"PRIu32" bytes\n", esp_get_minimum_free_heap_size());


    vTaskSuspend(NULL);
}