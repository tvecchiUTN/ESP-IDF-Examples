#include <stdio.h>

#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "esp_log.h"
#include "driver/touch_sens.h"

static const char* ERR_TAG = "Salida de errores";

#define SAMPLE_CFG 1

esp_err_t init_sens_touch(touch_sensor_handle_t *handle_sens_touch, touch_channel_handle_t *handle_chan_touch)
{
    if(!handle_sens_touch || handle_chan_touch)
    {
        return ESP_ERR_INVALID_ARG;
    }

    touch_sensor_sample_config_t cfg_sample_arr[SAMPLE_CFG] = {
        TOUCH_SENSOR_V1_DEFAULT_SAMPLE_CONFIG(100, 0, 3.3),
    };
    
    touch_sensor_config_t cfg_sens_touch = TOUCH_SENSOR_DEFAULT_BASIC_CONFIG(SAMPLE_CFG, cfg_sample_arr);

    esp_err_t err_cfg_sensT = touch_sensor_new_controller(&cfg_sens_touch, handle_sens_touch);
    if(err_cfg_sensT == ESP_ERR_NO_MEM)
    {
        ESP_LOGW(ERR_TAG, "Memoria insuficiente");
        return ESP_ERR_NO_MEM;
    }
    else if(err_cfg_sensT != ESP_OK)
    {
        ESP_LOGW(ERR_TAG, "Controlador ya en uso o argumentos invalidos");
        return ESP_FAIL;
    }
    
    touch_channel_config_t cfg_channel_sensT = {
        .abs_active_thresh = 1000,
        .charge_speed = TOUCH_CHARGE_SPEED_4,
        .group = TOUCH_CHAN_TRIG_GROUP_1,
        .init_charge_volt = TOUCH_INIT_CHARGE_VOLT_DEFAULT,
    };
    
    esp_err_t err_cfg_chan_sensT = touch_sensor_new_channel(*handle_sens_touch, 0, &cfg_channel_sensT, handle_chan_touch);
    if(err_cfg_chan_sensT == ESP_ERR_NO_MEM)
    {
        ESP_LOGW(ERR_TAG, "Memoria insuficiente");
        return ESP_ERR_NO_MEM;
    }
    else if(err_cfg_chan_sensT != ESP_OK)
    {
        ESP_LOGW(ERR_TAG, "Controlador ya en uso o argumentos invalidos");
        return ESP_FAIL;
    }
    else if(err_cfg_chan_sensT == ESP_ERR_INVALID_STATE)
    {
        ESP_LOGW(ERR_TAG, "Controlador touch no habilitado o ya activado");
        return ESP_FAIL;
    }

    return ESP_OK;
}

void app_main(void)
{

    while(1)
    {
        ESP_LOGI("TESTING", "Probando la placa");
        vTaskDelay(pdMS_TO_TICKS(1000));
    }

}
