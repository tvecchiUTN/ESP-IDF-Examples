#include <stdio.h>
#include <string.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "driver/uart.h"
#include "driver/gpio.h"
#include "esp_log.h"

// Configuración de Pines y Buffer
#define ECHO_TEST_TXD  17
#define ECHO_TEST_RXD  16
#define ECHO_TEST_RTS  (UART_PIN_NO_CHANGE)
#define ECHO_TEST_CTS  (UART_PIN_NO_CHANGE)

#define PORT_UART      UART_NUM_2
#define BUF_SIZE       1024

static const char *TAG = "UART_TEST";

void init_uart(void)
{
    // 1. Configuración de parámetros (Velocidad, bits, paridad)
    uart_config_t uart_config = {
        .baud_rate = 115200, //Velocidad de transmision
        .data_bits = UART_DATA_8_BITS, //Informacion, estandar 8 bits
        .parity    = UART_PARITY_DISABLE, //Comprobacion de errores simple, sin paridad
        .stop_bits = UART_STOP_BITS_1, //Indica final de paquete, 1 bit
        .flow_ctrl = UART_HW_FLOWCTRL_DISABLE, //Control de flujo por Hardware, solo usa Tx y Rx
        .source_clk = UART_SCLK_DEFAULT, //Reloj base para UART, el sistema elige cual
    };
    
    // Aplicamos la configuración al UART2
    ESP_ERROR_CHECK(uart_param_config(PORT_UART, &uart_config));

    // 2. Asignación de Pines (TX, RX, RTS, CTS)
    // En ESP32 puedes asignar cualquier periférico a cualquier pin (Matrix GPIO)
    //Los ultimos dos parametros son para el control de flujo
    ESP_ERROR_CHECK(uart_set_pin(PORT_UART, ECHO_TEST_TXD, ECHO_TEST_RXD, ECHO_TEST_RTS, ECHO_TEST_CTS));

    // 3. Instalación del Driver
    // - buffer rx: Asigna un espacio en la RAM, BUF_SIZE
    // - buffer tx: BUF_SIZE (0 bloquea hasta enviar)
    // - event queue: Sirve para recibir eventos, 0 (no usaremos cola de eventos por ahora)
    // - alloc flags: 0
    ESP_ERROR_CHECK(uart_driver_install(PORT_UART, BUF_SIZE * 2, 0, 0, NULL, 0));
    
    ESP_LOGI(TAG, "UART2 Inicializado en TX=%d, RX=%d", ECHO_TEST_TXD, ECHO_TEST_RXD);
}

void app_main(void)
{
    init_uart();

    // Mensaje a enviar
    const char *mensaje = "Ping desde ESP32";
    
    // Buffer para guardar lo que recibimos
    uint8_t *data = (uint8_t *) malloc(BUF_SIZE);

    while (1) {
        // --- 1. ENVIAR (WRITE) ---
        // Escribimos bytes en el UART. 
        int len_enviada = uart_write_bytes(PORT_UART, mensaje, strlen(mensaje));
        ESP_LOGI(TAG, "Enviado: %s (%d bytes)", mensaje, len_enviada);

        // --- 2. RECIBIR (READ) ---
        // Esperamos hasta 100ms (100 / portTICK_PERIOD_MS) a que lleguen datos
        int len_recibida = uart_read_bytes(PORT_UART, data, BUF_SIZE, pdMS_TO_TICKS(100));

        if (len_recibida > 0) {
            data[len_recibida] = '\0'; // Añadimos terminador null para imprimir como string
            ESP_LOGI(TAG, "Recibido (Loopback): %s", (char *)data);
        } else {
            ESP_LOGW(TAG, "No se recibió nada. ¿Conectaste el cable entre el Pin 16 y 17?");
        }

        vTaskDelay(pdMS_TO_TICKS(2000));
    }
    
    free(data); // (Nunca se alcanza en este ejemplo, pero es buena práctica en C)
}