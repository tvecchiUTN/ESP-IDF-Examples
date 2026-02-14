#include <stdio.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "esp_log.h"
#include "my_touch_driver.h"

#define CHAN_ID TOUCH_MIN_CHAN_ID

static const char *TAG = "EXAMPLE 06-Sensor Touch";

volatile bool isCalled = 0;

bool function_on_callback(touch_sensor_handle_t sens_handle, const touch_active_event_data_t *event, void *user_ctx)
{
    isCalled = 1;
    return true;
}

touch_sensor_sample_config_t sample_cfg[TOUCH_CFG_NUM] = {TOUCH_SENSOR_V1_DEFAULT_SAMPLE_CONFIG(5.0, TOUCH_VOLT_LIM_L_0V5, TOUCH_VOLT_LIM_H_1V7)};

void app_main(void)
{
    touch_sensor_handle_t handle_sensor_touch = NULL;
    touch_channel_handle_t channel_handle_sensor_touch = NULL;

    ESP_ERROR_CHECK(initial_setup_touch_sensor(sample_cfg, &handle_sensor_touch));
    
    ESP_ERROR_CHECK(setup_channel_sensor(handle_sensor_touch, CHAN_ID, &channel_handle_sensor_touch));

    ESP_ERROR_CHECK(setup_filter_sensor(handle_sensor_touch));

    ESP_ERROR_CHECK(initial_scan_sensor(handle_sensor_touch, channel_handle_sensor_touch, NULL));

    touch_event_callbacks_t callbacks_touch = {
        .on_active = function_on_callback,
    };

    ESP_ERROR_CHECK(touch_sensor_register_callbacks(handle_sensor_touch, &callbacks_touch, NULL));

    ESP_ERROR_CHECK(touch_sensor_enable(handle_sensor_touch));

    ESP_ERROR_CHECK(touch_sensor_start_continuous_scanning(handle_sensor_touch));

    uint32_t data[TOUCH_CFG_NUM] = {};

    while(1)
    {
        ESP_ERROR_CHECK(touch_channel_read_data(channel_handle_sensor_touch, TOUCH_CHAN_DATA_TYPE_SMOOTH, data));

        ESP_LOGI(TAG, "El valor es de %u", data[0]);

        if(isCalled)
        {
            ESP_LOGI(TAG, "Muy bien, se presiono el pin tactil");
            isCalled = 0;
        }

        vTaskDelay(pdMS_TO_TICKS(1000));
    }
}