#include <stdio.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "driver/ledc.h"
#include "esp_err.h"

// Configuración del Pin y PWM
#define LED_PIN 2
#define LEDC_TIMER              LEDC_TIMER_0
#define LEDC_MODE               LEDC_LOW_SPEED_MODE
#define LEDC_OUTPUT_IO          (LED_PIN) // Define el pin de salida
#define LEDC_CHANNEL            LEDC_CHANNEL_0
#define LEDC_DUTY_RES           LEDC_TIMER_13_BIT // Resolución de 13 bits (0 - 8191)
#define LEDC_FREQUENCY          5000 // Frecuencia en Hertz (5 kHz)

static const char *TAG = "PWM_EJEMPLO";

// --- FUNCIÓN DE INICIALIZACIÓN ---
void init_pwm_led(void)
{
    // 1. Configuración del Timer (El reloj base)
    ledc_timer_config_t ledc_timer = {
        .speed_mode       = LEDC_MODE,
        .timer_num        = LEDC_TIMER,
        .duty_resolution  = LEDC_DUTY_RES,
        .freq_hz          = LEDC_FREQUENCY,  // Frecuencia de salida
        .clk_cfg          = LEDC_AUTO_CLK
    };
    ESP_ERROR_CHECK(ledc_timer_config(&ledc_timer));

    // 2. Configuración del Canal (Vincula Timer -> GPIO)
    ledc_channel_config_t ledc_channel = {
        .speed_mode     = LEDC_MODE,
        .channel        = LEDC_CHANNEL,
        .timer_sel      = LEDC_TIMER,
        .intr_type      = LEDC_INTR_DISABLE,
        .gpio_num       = LEDC_OUTPUT_IO,
        .duty           = 0, // Inicia apagado (0% duty)
        .hpoint         = 0
    };
    ESP_ERROR_CHECK(ledc_channel_config(&ledc_channel));
}

void app_main(void)
{
    init_pwm_led();

    // Calculamos el valor máximo posible con 13 bits (2^13 - 1 = 8191)
    // Esto hace el código portable si cambias la resolución arriba.
    uint32_t max_duty = (1 << LEDC_DUTY_RES) - 1; 

    while (1) {
        
        // --- FADE IN (Subir brillo) ---
        printf("Subiendo brillo...\n");
        // Vamos de 0 a MaxDuty en pasos de 100
        for (int duty = 0; duty <= max_duty; duty += 200) {
            
            // 1. Establecer el nuevo valor objetivo
            ledc_set_duty(LEDC_MODE, LEDC_CHANNEL, duty);
            
            // 2. Aplicar el cambio (Hardware Trigger)
            ledc_update_duty(LEDC_MODE, LEDC_CHANNEL);
            
            // Pequeña espera para ver el efecto
            vTaskDelay(pdMS_TO_TICKS(20));
        }

        // --- FADE OUT (Bajar brillo) ---
        printf("Bajando brillo...\n");
        for (int duty = max_duty; duty >= 0; duty -= 200) {
            ledc_set_duty(LEDC_MODE, LEDC_CHANNEL, duty);
            ledc_update_duty(LEDC_MODE, LEDC_CHANNEL);
            vTaskDelay(pdMS_TO_TICKS(20));
        }
        
        vTaskDelay(pdMS_TO_TICKS(500)); // Espera un poco apagado
    }
}