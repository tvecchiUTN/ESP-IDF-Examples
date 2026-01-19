#pragma once

#ifndef __BUTTONINTR_H
#define __BUTTONINTR_H

#include "driver/gpio.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"

#define PININPUT 0 //Boton interno BOOT

//void IRAM_ATTR func_intr(void* args);

void button_intr_init(const SemaphoreHandle_t SemaHandle);

#endif