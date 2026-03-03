#pragma once

#include "freertos/FreeRTOS.h"
#include "freertos/task.h"

#include "driver/dac_oneshot.h"

esp_err_t init_dac_os(dac_channel_t chan_id, dac_oneshot_handle_t *handle_dac_os);