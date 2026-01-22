#include "app_pwm.h"

#define MY_LED_GPIO 4
#define MY_PWM_CHANNEL LEDC_CHANNEL_0

void app_main(void)
{
    // Ahora el main tiene el control de la configuración física
    esp_err_t err = pwm_init(MY_LED_GPIO, MY_PWM_CHANNEL);
    
    if (err != ESP_OK) {
        ESP_LOGW("MAIN", "Critical Failure initializing PWM. Halting.");
        while(1) {vTaskDelay(1000);} // Stop execution
    }

    while (1) 
    {
        pwm_auto_fading(MY_PWM_CHANNEL);
    }
}