#include <stdio.h>

#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "esp_log.h"
#include "driver/touch_sens.h"

static const char* ERR_TAG = "Salida de errores";

esp_err_t init_sens_touch(touch_sensor_handle_t *handle_sens_touch)
{
    if(!handle_sens_touch)
    {
        return ESP_ERR_INVALID_ARG;
    }

    touch_sensor_sample_config_t cfg_sample_arr[1] = {
        TOUCH_SENSOR_V1_DEFAULT_SAMPLE_CONFIG(100, 0, 3.3),
    };

    
    touch_sensor_config_t cfg_sens_touch = {
        .intr_trig_group = TOUCH_INTR_TRIG_GROUP_1,
        .intr_trig_mode = TOUCH_INTR_TRIG_ON_BELOW_THRESH,
        .meas_interval_us = 10000,
        .power_on_wait_us = 0,
        .sample_cfg = cfg_sample_arr,
        .sample_cfg_num = 1,
    };

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

    /*
    touch_channel_config_t cfg_channel_sensT = {
        .abs_active_thresh = 1000,
        .charge_speed = TOUCH_CHARGE_SPEED_4,
        .group = TOUCH_CHAN_TRIG_GROUP_1,
        .init_charge_volt = TOUCH_INIT_CHARGE_VOLT_DEFAULT,
    };
    */

    return ESP_OK;
}

void app_main(void)
{

}
