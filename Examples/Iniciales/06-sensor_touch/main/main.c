#include <stdio.h>

#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "esp_log.h"
#include "esp_check.h"
#include "driver/touch_sens.h"

//static const char* ERR_TAG = "Salida de errores - modulo touch pad";

static const char* TAG = "TOUCH_DRIVER";

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

    ESP_RETURN_ON_ERROR(err, TAG, "Fallo al crear controlador global");

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
    
    ESP_RETURN_ON_ERROR(err, TAG, "Fallo al crear canal %d", chan_id);

    return ESP_OK;
}

static esp_err_t init_filter_touch_pad(touch_sensor_handle_t handle_touch_sensor)
{
    if(!handle_touch_sensor)
    {
        return ESP_ERR_INVALID_ARG;
    }

    touch_sensor_filter_config_t cfg_filter_touch = {
        .interval_ms = 10,
        .data_filter_fn = NULL,
    };

    return touch_sensor_config_filter(handle_touch_sensor, &cfg_filter_touch);
}

static esp_err_t calibrate_threshold(touch_sensor_handle_t handle_sensor_touch, touch_channel_handle_t handle_channel_sensor, uint32_t *out_threshold)
{
    if(!out_threshold)
    {
        return ESP_ERR_INVALID_ARG;
    }

    uint32_t benchmark_val[1];

    ESP_RETURN_ON_ERROR(touch_channel_read_data(handle_channel_sensor, TOUCH_CHAN_DATA_TYPE_RAW, benchmark_val), TAG, "Fallo al leer");

    *out_threshold = (uint32_t)(benchmark_val[0] * 0.8);

    touch_sensor_stop_continuous_scanning(handle_sensor_touch);
    touch_sensor_disable(handle_sensor_touch);

    touch_channel_config_t recfg_chan = {0};
    recfg_chan.abs_active_thresh[0] = *out_threshold;

    touch_sensor_reconfig_channel(handle_channel_sensor, &recfg_chan);

    touch_sensor_start_continuous_scanning(handle_sensor_touch);
    touch_sensor_enable(handle_sensor_touch);

    return ESP_OK;
}

#define CHAN_USER 3

void app_main(void)
{
    touch_sensor_handle_t handle_sensor_touch = NULL;
    touch_channel_handle_t handle_channel_0 = NULL;
    uint32_t threshold = 0;

    ESP_ERROR_CHECK(init_controller(&handle_sensor_touch));

    ESP_ERROR_CHECK(init_filter_touch_pad(handle_sensor_touch));

    ESP_ERROR_CHECK(init_touch_pad(CHAN_USER, handle_sensor_touch, &handle_channel_0));

    ESP_ERROR_CHECK(touch_sensor_enable(handle_sensor_touch));

    ESP_ERROR_CHECK(touch_sensor_start_continuous_scanning(handle_sensor_touch));

    vTaskDelay(pdMS_TO_TICKS(500));

    ESP_ERROR_CHECK(calibrate_threshold(handle_sensor_touch, handle_channel_0, &threshold));

    uint32_t current_val[1];

    while(1)
    {
        if(touch_channel_read_data(handle_channel_0, TOUCH_CHAN_DATA_TYPE_RAW, current_val) == ESP_OK)
        {
            if(current_val[0] < threshold)
            {
                ESP_LOGI(TAG, "TOCADO! Val: %lu < Thr: %lu", current_val[0], threshold);
            }
        }

        ESP_LOGI(TAG, "El valor actual es %d", current_val[0]);

        vTaskDelay(pdMS_TO_TICKS(100));
    }
}
