#include <stdio.h>

#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "esp_log.h"
#include "esp_check.h"
#include "driver/touch_pad.h"

static const char* ERR_TAG = "Salida de errores";

static esp_err_t init_touch(uint16_t *init_threshold, touch_pad_t touch_num)
{
    if(!init_threshold)
    {
        return ESP_ERR_INVALID_ARG;
    }
    if(touch_num >= TOUCH_PAD_MAX)
    {
        return ESP_ERR_INVALID_ARG;
    }

    
    ESP_RETURN_ON_ERROR(touch_pad_init(), ERR_TAG, "Fallo al inicializar driver");

    ESP_RETURN_ON_ERROR(touch_pad_read(touch_num, init_threshold), ERR_TAG, "Error al leer valor del pin tactil %d", touch_num);

    //Dato, el threashold es cercano a init_threshold, por eso se hace un calculo antes de ussarlo. Lo que me da esto es el valor "ambiente"
}

void app_main(void)
{


    while(1)
    {
        ESP_LOGI("TESTING", "Probando la placa");
        vTaskDelay(pdMS_TO_TICKS(1000));
    }

}
