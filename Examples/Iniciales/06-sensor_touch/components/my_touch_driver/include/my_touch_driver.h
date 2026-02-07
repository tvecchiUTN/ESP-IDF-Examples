#pragma once

#include "driver/touch_sens.h"

#define TOUCH_CFG_NUM 1
#define RATIO_BENCKMARK 0.015

#define TOUCH_CHAN_DEFAULT_CFG()        {  \
    .abs_active_thresh = {1000},  \
    .charge_speed = TOUCH_CHARGE_SPEED_7,  \
    .init_charge_volt = TOUCH_INIT_CHARGE_VOLT_DEFAULT,  \
    .group = TOUCH_CHAN_TRIG_GROUP_BOTH,  \
}

void initial_setup_touch_sensor(touch_sensor_handle_t *sens_handle);

void setup_channel_sensor(touch_sensor_handle_t sens_handle, int chan_id, touch_channel_handle_t *chan_handle);

void setup_filter_sensor(touch_sensor_handle_t sens_handle);

void inital_scan_sensor(touch_sensor_handle_t sens_handle, touch_channel_handle_t chan_handle, uint32_t *threshold);