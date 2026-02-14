#include "my_touch_driver.h"
#include "esp_log.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"

esp_err_t initial_setup_touch_sensor(touch_sensor_sample_config_t *arr_sample_cfg, touch_sensor_handle_t *sens_handle)
{
    touch_sensor_config_t cfg_sens_touch = TOUCH_SENSOR_DEFAULT_BASIC_CONFIG(TOUCH_CFG_NUM, arr_sample_cfg);

    esp_err_t err = touch_sensor_new_controller(&cfg_sens_touch, sens_handle);

    return err;
}

esp_err_t setup_channel_sensor(touch_sensor_handle_t sens_handle, int chan_id, touch_channel_handle_t *chan_handle)
{
    touch_channel_config_t cfg_channel_sensor = TOUCH_CHAN_DEFAULT_CFG();
    cfg_channel_sensor.abs_active_thresh[0] = 1000;

    esp_err_t err = touch_sensor_new_channel(sens_handle, chan_id, &cfg_channel_sensor, chan_handle);

    return err;
}

esp_err_t setup_filter_sensor(touch_sensor_handle_t sens_handle)
{
    touch_sensor_filter_config_t filter_cfg = TOUCH_SENSOR_DEFAULT_FILTER_CONFIG();
    esp_err_t err = touch_sensor_config_filter(sens_handle, &filter_cfg);

    return err;
}

esp_err_t initial_scan_sensor(touch_sensor_handle_t sens_handle, touch_channel_handle_t chan_handle, uint32_t *threshold)
{
    esp_err_t err;

    err = touch_sensor_enable(sens_handle);
    if((err != ESP_OK) && (err != ESP_ERR_INVALID_STATE))
    {
        return err;
    }

    err = touch_sensor_trigger_oneshot_scanning(sens_handle, 2000);
    if(err != ESP_OK)
    {
        return err;
    }

    vTaskDelay(pdMS_TO_TICKS(100));

    uint32_t benchmark[TOUCH_CFG_NUM] = {};

    err = touch_channel_read_data(chan_handle, TOUCH_CHAN_DATA_TYPE_SMOOTH, benchmark);
    if(err != ESP_OK)
    {
        return err;
    }

    err = touch_sensor_disable(sens_handle);
    if((err != ESP_OK) && (err != ESP_ERR_INVALID_STATE))
    {
        return err;
    }

    if(!benchmark[0])
    {
        ESP_LOGW("TOUCH", "Benchmark read 0, hardware not ready?");
        return ESP_FAIL;
    }

    touch_channel_config_t cfg_channel_sensor = TOUCH_CHAN_DEFAULT_CFG();

    uint32_t delta = (uint32_t)(benchmark[0] * RATIO_BENCHMARK);
    cfg_channel_sensor.abs_active_thresh[0] = benchmark[0] - delta;
    if(threshold)
    {
        *threshold = cfg_channel_sensor.abs_active_thresh[0];
    }

    err = touch_sensor_reconfig_channel(chan_handle, &cfg_channel_sensor);

    return err;
}