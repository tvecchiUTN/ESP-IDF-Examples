#include <stdio.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "freertos/queue.h"
#include "driver/gpio.h"
#include "esp_log.h"

#define PIN_BUTTON 0
static const char *TAG = "ISR_DEMO";

// Cola para comunicar la ISR con el Main
QueueHandle_t gpio_evt_queue = NULL;

// --- 1. EL MANEJADOR DE INTERRUPCIÓN (ISR) ---
// IRAM_ATTR: Obliga a esta función a cargarse en la RAM interna (Rápida/Segura)
static void IRAM_ATTR gpio_isr_handler(void* arg)
{
    uint32_t gpio_num = (uint32_t) arg;
    
    // Variable para saber si debemos cambiar de contexto (despertar al main inmediatamente)
    BaseType_t xHigherPriorityTaskWoken = pdFALSE;
    
    // Enviamos el número del pin que disparó la interrupción
    // ¡OJO! Usamos la versión "FromISR"
    xQueueSendFromISR(gpio_evt_queue, &gpio_num, &xHigherPriorityTaskWoken);
    
    // Si enviar el mensaje despertó a una tarea más importante (el main),
    // forzamos el cambio de contexto ya mismo.
    if (xHigherPriorityTaskWoken) {
        portYIELD_FROM_ISR();
    }
}

// --- 2. CONFIGURACIÓN ---
void init_button(void)
{
    gpio_config_t io_conf = {};

    // Interrupción: Flanco de bajada (Cuando pasas de soltar a apretar)
    // HIGH_LEVEL es peligroso en botones, dispararía miles de veces por segundo mientras aprietas.
    io_conf.intr_type = GPIO_INTR_NEGEDGE;
    
    io_conf.mode = GPIO_MODE_INPUT;
    io_conf.pin_bit_mask = (1ULL << PIN_BUTTON);
    io_conf.pull_down_en = 0;
    io_conf.pull_up_en = 1; // Pull-up porque el botón conecta a GND
    
    ESP_ERROR_CHECK(gpio_config(&io_conf));

    // A. Instalamos el servicio global de interrupciones GPIO
    // (Permite gestionar interrupciones pin por pin)
    ESP_ERROR_CHECK(gpio_install_isr_service(0));

    // B. Añadimos nuestro pin al gestor
    // Pasamos (void*) PIN_BUTTON como argumento para que el handler sepa qué pin fue
    ESP_ERROR_CHECK(gpio_isr_handler_add(PIN_BUTTON, gpio_isr_handler, (void*) PIN_BUTTON));
    
    ESP_LOGI(TAG, "Botón configurado con Interrupciones.");
}

void app_main(void)
{
    // Crear la cola. Guardará enteros (el número de pin).
    // Profundidad: 10 elementos. Tamaño: sizeof(uint32_t).
    gpio_evt_queue = xQueueCreate(10, sizeof(uint32_t));

    init_button();

    uint32_t pin_recibido;

    while (1) {
        // --- 3. ESPERAR EVENTO (Consumidor) ---
        // xQueueReceive bloquea esta tarea hasta que llegue algo.
        // portMAX_DELAY significa "espera infinita" (0% uso de CPU mientras espera).
        if (xQueueReceive(gpio_evt_queue, &pin_recibido, portMAX_DELAY)) {
            
            ESP_LOGI(TAG, "¡Interrupción recibida del GPIO %lu!", pin_recibido);
            
            // Aquí podrías procesar lógica pesada que no se puede hacer en la ISR
        }
    }
}