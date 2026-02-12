#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "esp_err.h"
#include "esp_log.h"
#include "driver/uart.h"

#define UART_COM UART_NUM_1

#define UART_TXD 18
#define UART_RXD 19

#define UART_DEFAULT_CONFIG {              \
    .baud_rate = 115200,                   \
    .data_bits = UART_DATA_8_BITS,         \
    .parity = UART_PARITY_DISABLE,         \
    .stop_bits = UART_STOP_BITS_1,         \
    .flow_ctrl = UART_HW_FLOWCTRL_DISABLE, \
    .source_clk = UART_SCLK_DEFAULT,       \
    .rx_flow_ctrl_thresh = 122,            \
}

static const char *TAG = "Ejemplo 07-UART";

static void uart_event_task(void *param)
{
    QueueHandle_t uart_queue = (QueueHandle_t)param;

    uint8_t *msg_rcv = NULL;

    while (1)
    {
        uart_event_t event = {0};

        xQueueReceive(uart_queue, &event, portMAX_DELAY);

        switch (event.type)
        {
        case UART_DATA:
            uint32_t len = event.size;
            msg_rcv = malloc(64);
            if (!msg_rcv)
            {
                // Chequeo si no hay memoria, en un caso real, crear variable auxuiliar en el stack o cancelar aplicacion
            }

            uart_read_bytes(UART_COM, (void *)msg_rcv, len, portMAX_DELAY);

            msg_rcv[len] = '\0';

            ESP_LOGI(TAG, "El mensaje es %s", msg_rcv);

            free(msg_rcv);
            break;

        case UART_BREAK:
            ESP_LOGI(TAG, "UART Break detected");
            break;

        case UART_BUFFER_FULL:
        case UART_FIFO_OVF:
            uart_flush_input(UART_COM);
            xQueueReset(uart_queue);
            break;

        case UART_FRAME_ERR:
            ESP_LOGE(TAG, "UART Frame Error");
            break;

        case UART_PARITY_ERR:
            ESP_LOGE(TAG, "UART Parity Error");
            break;

        default:
            ESP_LOGI(TAG, "Evento UART desconocido: %d", event.type);
            break;
        }
    }
}

const char *msg_send = "Hola como estan, esto se envio mediante el comando UART";

void app_main(void)
{
    uart_config_t cfg_uart_com = UART_DEFAULT_CONFIG;

    ESP_ERROR_CHECK(uart_param_config(UART_COM, &cfg_uart_com));

    ESP_ERROR_CHECK(uart_set_pin(UART_COM, UART_TXD, UART_RXD, UART_PIN_NO_CHANGE, UART_PIN_NO_CHANGE));

    QueueHandle_t handle_uart_queue = NULL;

    esp_err_t err = uart_driver_install(UART_COM, 2048, 2048, 20, &handle_uart_queue, 0);

    ESP_ERROR_CHECK(err);

    TaskHandle_t handle_uart_task = NULL;

    xTaskCreate(uart_event_task, "Task UART", 2048, (void *)handle_uart_queue, 1, &handle_uart_task);
    if (!handle_uart_task)
    {
        ESP_LOGW(TAG, "No se pudo crear la tarea");
    }

    while (1)
    {
        uart_write_bytes(UART_COM, (const void *)msg_send, strlen(msg_send));

        vTaskDelay(pdMS_TO_TICKS(3000));
    }
}
