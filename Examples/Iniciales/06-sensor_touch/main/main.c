#include <stdio.h>

#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "esp_log.h"
#include "esp_check.h"
#include "driver/touch_sens.h"

static const char* ERR_TAG = "Salida de errores - modulo touch pad";

#define SAMPLE_NUM 1

static touch_sensor_sample_config_t sample_cfg[SAMPLE_NUM] = {TOUCH_SENSOR_V1_DEFAULT_SAMPLE_CONFIG(2, 10, 10)};

static esp_err_t init_controller(touch_sensor_handle_t *handle_touch_sensor)
{
    if(!handle_touch_sensor)
    {
        return ESP_ERR_INVALID_ARG;
    }

    touch_sensor_config_t cfg_touch = TOUCH_SENSOR_DEFAULT_BASIC_CONFIG(SAMPLE_NUM, sample_cfg);

    esp_err_t err;
    err = touch_sensor_new_controller(&cfg_touch, handle_touch_sensor);

    ESP_RETURN_ON_ERROR(err, ERR_TAG, "Fallo al crear controlador global");

    return ESP_OK;
}

static esp_err_t init_touch_pad(int chan_id, touch_sensor_handle_t handle_touch_sensor, touch_channel_handle_t *handle_channel_sensor)
{
    if(!handle_channel_sensor || !handle_touch_sensor)
    {
        return ESP_ERR_INVALID_ARG;
    }

    touch_channel_config_t cfg_channel_touch = {0};
    cfg_channel_touch.abs_active_thresh[0] = 100;
    

    esp_err_t err = touch_sensor_new_channel(handle_touch_sensor, chan_id, &cfg_channel_touch, handle_channel_sensor);
    
    ESP_RETURN_ON_ERROR(err, ERR_TAG, "Fallo al crear canal %d", chan_id);

    return ESP_OK;
}

void app_main(void)
{
    touch_sensor_handle_t handle_sensor_touch = NULL;
    touch_channel_handle_t handle_channel_0 = NULL;

    ESP_ERROR_CHECK(init_controller(&handle_sensor_touch));

    ESP_ERROR_CHECK(init_touch_pad(0, handle_sensor_touch, &handle_channel_0));

    while(1)
    {
        ESP_LOGI("TESTING", "Probando la placa");
        vTaskDelay(pdMS_TO_TICKS(1000));
    }
}
