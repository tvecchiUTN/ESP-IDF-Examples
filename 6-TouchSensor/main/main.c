#include <stdio.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "driver/touch_pad.h" // Librería correcta para ESP32 Clásico
#include "esp_log.h"

#define TOUCH_PIN_NUM TOUCH_PAD_NUM0  // GPIO 4
static const char *TAG = "TOUCH_DEMO";

// Umbral: Si el valor cae por debajo de esto, se considera tocado.
// (Se calibra en el init)
static uint32_t touch_threshold = 0;

void init_touch_sensor(void)
{
    // 1. Inicializar el driver global
    // No requiere estructura compleja, solo inicia la máquina de estados.
    ESP_ERROR_CHECK(touch_pad_init());

    // 2. Configurar el voltaje de referencia (Rango dinámico)
    // TOUCH_HVOLT_2V7 y LVOLT_0V5 son los valores más estables ("Valores más usados")
    touch_pad_set_voltage(TOUCH_HVOLT_2V7, TOUCH_LVOLT_0V5, TOUCH_HVOLT_ATTEN_1V);

    // 3. Configurar el pad específico (GPIO 4)
    ESP_ERROR_CHECK(touch_pad_config(TOUCH_PIN_NUM, 0));

    // --- CALIBRACIÓN ---
    // Leemos el valor "en vacío" (sin tocar) para saber cuál es la base.
    uint16_t touch_value;
    touch_pad_read(TOUCH_PIN_NUM, &touch_value);
    
    // Regla de oro: El umbral suele ser 2/3 del valor sin tocar.
    touch_threshold = touch_value * 2 / 3;
    
    ESP_LOGI(TAG, "Touch inicializado. Valor base: %d, Umbral disparo: %lu", touch_value, touch_threshold);
}

void app_main(void)
{
    init_touch_sensor();

    while (1) {
        uint16_t touch_value;
        
        // 4. Lectura (Polling)
        // Pasamos la dirección (&) de la variable para que la función escriba en ella.
        ESP_ERROR_CHECK(touch_pad_read(TOUCH_PIN_NUM, &touch_value));

        // Lógica Inversa: Tocar = Baja capacitancia = Valor Menor
        if (touch_value < touch_threshold) {
            ESP_LOGI(TAG, "¡TOCADO! Val: %d", touch_value);
            // Aquí encenderías un LED
        } else {
            // Comentar para no saturar
            // ESP_LOGI(TAG, "Libre. Val: %d", touch_value); 
        }

        vTaskDelay(pdMS_TO_TICKS(100));
    }
}