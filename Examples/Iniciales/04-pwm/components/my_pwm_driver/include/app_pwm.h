/**
 * @file app_pwm.h
 * @brief LEDC PWM Control Driver wrapper for ESP32.
 *
 * This component provides an abstraction layer over the native ESP-IDF LEDC driver,
 * simplifying the initialization and control of PWM signals for LED fading effects.
 *
 * @author Vecchi Tomas
 * @date 2023-10-27
 */

#pragma once

#include "driver/ledc.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "esp_log.h"

// --- Configuration Constants ---
// Exposed constants for transparency, though usually passed as arguments in advanced drivers
#define PWM_TIMER LEDC_TIMER_0
#define PWM_MODE LEDC_LOW_SPEED_MODE
#define PWM_DUTY_RES LEDC_TIMER_13_BIT // 13 bit: 0 - 8191
#define PWM_FREQUENCY 5000             // 5 kHz

/**
 * @brief Maximum duty cycle value based on the selected resolution.
 * Calculated as (2 ^ resolution) - 1.
 */
extern const uint32_t LIMIT_LEDC;

/**
 * @brief Initialize the LEDC Timer and Channel.
 *
 * Configures the hardware timer and assigns a specific GPIO to a PWM channel.
 *
 * @param[in] gpio_num The physical GPIO pin number to output PWM.
 * @param[in] channel  The LEDC channel (0-7) to assign to this pin.
 * @param[in] installFade If true, install the fade function needed for auto fading
 *
 * @return
 * - ESP_OK: Success
 * - ESP_ERR_INVALID_ARG: Parameter error
 * - ESP_FAIL: Driver configuration failed
 */
esp_err_t pwm_init(int gpio_num, ledc_channel_t channel, bool installFade);

/**
 * @brief Performs a manual fading effect using CPU delays.
 *
 * @note This function blocks the calling task. Use for testing/demo only.
 *
 * @param[in] channel The LEDC channel to control.
 * @return esp_err_t ESP_OK on success.
 */
esp_err_t pwm_manual_fading(ledc_channel_t channel);

/**
 * @brief Starts a hardware-controlled fading effect.
 *
 * This function is non-blocking. It configures the hardware to fade
 * independently of the CPU.
 *
 * @param[in] channel The LEDC channel to control.
 * @return
 * - ESP_OK: Success
 * - ESP_ERR_INVALID_STATE: Fade function not installed
 */
esp_err_t pwm_auto_fading(ledc_channel_t channel);