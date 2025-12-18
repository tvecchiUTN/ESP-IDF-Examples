#include <stdio.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "driver/gpio.h"
#include "esp_log.h"

// Definimos el Pin
#define LED_PIN 2
static const char *TAG = "Example blinker";

// --- FUNCIÓN DE INICIALIZACIÓN (Driver) ---
// Aquí encapsulamos la configuración "fea" o compleja del hardware
void init_led_gpio(void)
{
    // Preparamos la estructura de configuración
    // Esto es lo que Arduino hace por ti en "pinMode", pero aquí tienes control total
    gpio_config_t io_conf = {};

    io_conf.intr_type = GPIO_INTR_DISABLE;      // No queremos interrupciones
    io_conf.mode = GPIO_MODE_OUTPUT;            // Modo salida
    //Configuras dos pines seria (1ULL << PIN1) | (1ULL << PIN2) | etc
    io_conf.pin_bit_mask = (1ULL << LED_PIN);   // Bitmask: Seleccionamos el PIN 2
    io_conf.pull_down_en = 0;                   // Desactivar resistencia pull-down
    io_conf.pull_up_en = 0;                     // Desactivar resistencia pull-up
    
    // Aplicamos la configuración
    gpio_config(&io_conf);
    
    ESP_LOGI(TAG, "GPIO %d configurado como salida", LED_PIN);
}

// --- APP MAIN ---
void app_main(void)
{
    // 1. Inicializar Hardware
    init_led_gpio();

    int estado_led = 0;

    // 2. Bucle Principal
    while (1) {
        // Lógica de aplicación
        estado_led = !estado_led; // Invertir estado (toggle)
        
        // Acción directa sobre el hardware
        gpio_set_level(LED_PIN, estado_led);
        
        // Log informativo
        ESP_LOGI(TAG, "El LED está: %s", estado_led ? "ENCENDIDO" : "APAGADO");

        // Delay no bloqueante (permite que el WiFi funcione de fondo si estuviera activo)
        vTaskDelay(pdMS_TO_TICKS(1000));
    }
}