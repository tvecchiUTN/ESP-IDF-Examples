#pragma once

#ifndef __LED_INIT_H
#define __LED_INIT_H

#include <stdio.h>
#include "driver/gpio.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "esp_log.h"
#include "sdkconfig.h"

esp_err_t init_gpio(void);

#endif