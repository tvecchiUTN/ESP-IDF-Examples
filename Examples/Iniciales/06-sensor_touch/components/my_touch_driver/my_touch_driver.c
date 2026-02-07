#include "my_touch_driver.h"
#include "esp_log.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "esp_log.h"
//#include "esp_err"

touch_sensor_sample_config_t sample_cfg[TOUCH_CFG_NUM] = {TOUCH_SENSOR_V1_DEFAULT_SAMPLE_CONFIG(5.0, TOUCH_VOLT_LIM_L_0V5, TOUCH_VOLT_LIM_H_1V7)};

void initial_setup_touch_sensor(touch_sensor_handle_t *sens_handle)
{
    touch_sensor_config_t cfg_sens_touch = TOUCH_SENSOR_DEFAULT_BASIC_CONFIG(TOUCH_CFG_NUM, sample_cfg);

    ESP_ERROR_CHECK(touch_sensor_new_controller(&cfg_sens_touch, sens_handle));
}

void setup_channel_sensor(touch_sensor_handle_t sens_handle, int chan_id, touch_channel_handle_t *chan_handle)
{
    touch_channel_config_t cfg_channel_sensor = TOUCH_CHAN_DEFAULT_CFG();
    cfg_channel_sensor.abs_active_thresh[0] = 1000;

    ESP_ERROR_CHECK(touch_sensor_new_channel(sens_handle, chan_id, &cfg_channel_sensor, chan_handle));
}

void setup_filter_sensor(touch_sensor_handle_t sens_handle)
{
    touch_sensor_filter_config_t filter_cfg = TOUCH_SENSOR_DEFAULT_FILTER_CONFIG();
    ESP_ERROR_CHECK(touch_sensor_config_filter(sens_handle, &filter_cfg));
}

void inital_scan_sensor(touch_sensor_handle_t sens_handle, touch_channel_handle_t chan_handle, uint32_t *threshold)
{
    ESP_ERROR_CHECK(touch_sensor_enable(sens_handle));

    ESP_ERROR_CHECK(touch_sensor_trigger_oneshot_scanning(sens_handle, -1));

    ESP_ERROR_CHECK(touch_sensor_disable(sens_handle));

    uint32_t benchmark[TOUCH_CFG_NUM] = {};

    ESP_ERROR_CHECK(touch_channel_read_data(chan_handle, TOUCH_CHAN_DATA_TYPE_SMOOTH, benchmark));

    touch_channel_config_t cfg_channel_sensor = TOUCH_CHAN_DEFAULT_CFG();
    cfg_channel_sensor.abs_active_thresh[0] = (uint32_t)(benchmark[0] * (1 - RATIO_BENCKMARK));
    *threshold =(uint32_t)(benchmark[0] * (1 - RATIO_BENCKMARK));

    ESP_ERROR_CHECK(touch_sensor_reconfig_channel(chan_handle, &cfg_channel_sensor));
}