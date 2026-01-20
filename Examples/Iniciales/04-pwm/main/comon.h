#pragma once

#ifndef __COMON_H
#define __COMON_H

#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "esp_log.h"
#include "esp_err.h"
#include "driver/ledc.h"

#define RES_LEDC LEDC_TIMER_13_BIT
#define FREC_LEDC 5000
#define TIMER_LEDC LEDC_TIMER_0
#define CHAN_LEDC LEDC_CHANNEL_0
#define SPEED_LEDC LEDC_LOW_SPEED_MODE

#define GPIO_LEDC 4

extern const uint32_t LIMIT_LEDC;

#endif