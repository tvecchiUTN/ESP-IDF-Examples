#include <stdio.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "driver/gpio.h"
#include "esp_log.h"

// Definimos pines
#define GPIO_BUTTON 0  // El botón BOOT en la mayoría de devkits
#define GPIO_LED    2  // El LED azul integrado

static const char *TAG = "EJEMPLO_INPUT";

// Función de configuración unificada
void init_hw(void)
{
    // --- 1. Configurar el LED (Salida) ---
    gpio_config_t io_conf_led = {};
    io_conf_led.intr_type = GPIO_INTR_DISABLE;
    io_conf_led.mode = GPIO_MODE_OUTPUT;
    io_conf_led.pin_bit_mask = (1ULL << GPIO_LED);
    io_conf_led.pull_down_en = 0;
    io_conf_led.pull_up_en = 0;
    gpio_config(&io_conf_led);

    // --- 2. Configurar el Botón (Entrada) ---
    gpio_config_t io_conf_btn = {};
    
    // Deshabilitamos interrupciones (por ahora leemos por encuesta/polling)
    io_conf_btn.intr_type = GPIO_INTR_DISABLE;
    
    // Modo entrada
    io_conf_btn.mode = GPIO_MODE_INPUT;
    
    // Máscara de bits para el pin del botón
    io_conf_btn.pin_bit_mask = (1ULL << GPIO_BUTTON);
    
    // Habilitamos Pull-up interno (Crucial para leer botones conectados a GND)
    io_conf_btn.pull_up_en = 0;
    io_conf_btn.pull_down_en = 1;
    
    gpio_config(&io_conf_btn);
    
    ESP_LOGI(TAG, "Hardware inicializado. Presiona el botón BOOT (GPIO 0).");
}

void app_main(void)
{
    init_hw();

    int last_state = -1; // Para imprimir solo cuando cambie

    while (1) {
        // --- Lectura Digital (Equivalente a digitalRead) ---
        int button_state = gpio_get_level(GPIO_BUTTON);

        // Lógica: Si usas Pull-up, el botón presionado lee 0 (LOW), suelto lee 1 (HIGH)
        if (button_state == 0) {
            gpio_set_level(GPIO_LED, 1); // Encender LED
            if (last_state != 0) {
                ESP_LOGI(TAG, "Botón PRESIONADO (Nivel Bajo)");
                last_state = 0;
            }
        } else {
            gpio_set_level(GPIO_LED, 0); // Apagar LED
            if (last_state != 1) {
                ESP_LOGI(TAG, "Botón SUELTO (Nivel Alto)");
                last_state = 1;
            }
        }

        // Pequeño delay para "debounce" simple y para no saturar el Watchdog
        vTaskDelay(pdMS_TO_TICKS(10));
    }
}