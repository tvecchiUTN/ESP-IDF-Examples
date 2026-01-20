#pragma once

#ifndef __AUTO_FADE_H
#define __AUTO_FADE_H

#include "comon.h"

esp_err_t init_ledc_fade();

esp_err_t auto_fading();

#endif