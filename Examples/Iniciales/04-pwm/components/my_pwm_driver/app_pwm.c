#include "app_pwm.h"
// Standard Tag for logging
static const char *TAG = "PWM_DRIVER";

// Global constant definition
const uint32_t LIMIT_LEDC = (1UL << PWM_DUTY_RES) - 1;

esp_err_t pwm_init(int gpio_num, ledc_channel_t channel, bool installFade)
{
    // 1. Timer Configuration (Shared resource)
    // In a real multi-channel scenario, we might check if timer is already init.
    ledc_timer_config_t cfg_timer = {
        .clk_cfg = LEDC_AUTO_CLK,
        .deconfigure = false,
        .duty_resolution = PWM_DUTY_RES,
        .freq_hz = PWM_FREQUENCY,
        .speed_mode = PWM_MODE,
        .timer_num = PWM_TIMER};

    esp_err_t err = ledc_timer_config(&cfg_timer);
    if (err != ESP_OK)
    {
        ESP_LOGE(TAG, "Timer config failed: %s", esp_err_to_name(err));
        return err;
    }

    // 2. Channel Configuration (Specific resource)
    ledc_channel_config_t cfg_channel = {
        .channel = channel,
        .duty = 0,
        .flags.output_invert = 0,
        .gpio_num = gpio_num, // <--- Using argument
        .hpoint = 0,
        .intr_type = LEDC_INTR_DISABLE,
        .sleep_mode = LEDC_SLEEP_MODE_NO_ALIVE_NO_PD,
        .speed_mode = PWM_MODE,
        .timer_sel = PWM_TIMER};

    err = ledc_channel_config(&cfg_channel);
    if (err != ESP_OK)
    {
        ESP_LOGE(TAG, "Channel config failed: %s", esp_err_to_name(err));
        return err;
    }

    // 3. Install Fade Service (needed for auto fading)
    if (installFade)
    {
        // ESP-IDF handles double-installation internally, so this is safe-ish,
        // but typically done once in system init.
        ledc_fade_func_install(0);
    }

    ESP_LOGI(TAG, "PWM initialized on GPIO %d, Channel %d", gpio_num, channel);
    return ESP_OK;
}

static inline esp_err_t setting_ledc_man(ledc_channel_t channel, uint32_t duty)
{
    esp_err_t err_set = ledc_set_duty(PWM_MODE, channel, duty);
    if (err_set != ESP_OK)
    {
        ESP_LOGW("DEBUG", "Arguments error");
        return err_set;
    }

    esp_err_t err_upd = ledc_update_duty(PWM_MODE, channel);
    if (err_upd != ESP_OK)
    {
        ESP_LOGW("DEBUG", "Arguments error");
        return err_upd;
    }

    vTaskDelay(1);

    return ESP_OK;
}

esp_err_t pwm_manual_fading(ledc_channel_t channel)
{
    for (int duty = 0; duty < LIMIT_LEDC; duty += 100)
    {
        setting_ledc_man(channel, duty);
    }

    vTaskDelay(pdMS_TO_TICKS(1000));

    for (int duty = LIMIT_LEDC; duty > 0; duty -= 100)
    {
        setting_ledc_man(channel, duty);
    }

    vTaskDelay(pdMS_TO_TICKS(1000));

    return ESP_OK;
}

static inline esp_err_t setting_fade_auto(ledc_channel_t channel, uint32_t target)
{
    esp_err_t err_set = ledc_set_fade_with_time(PWM_MODE, channel, target, 1000);
    if (err_set != ESP_OK)
    {
        ESP_LOGW("DEBUG", "Error seteando el fading: %s", esp_err_to_name(err_set));
        return err_set;
    }

    esp_err_t err_start = ledc_fade_start(PWM_MODE, channel, LEDC_FADE_NO_WAIT);
    if (err_start != ESP_OK)
    {
        ESP_LOGW("DEBUG", "Error iniciando el fading: %s", esp_err_to_name(err_set));
        return err_start;
    }

    return ESP_OK;
}

esp_err_t pwm_auto_fading(ledc_channel_t channel)
{
    // Ida
    if (setting_fade_auto(channel, LIMIT_LEDC) != ESP_OK)
    {
        return ESP_FAIL;
    }
    vTaskDelay(pdMS_TO_TICKS(1000 + 10));

    // Vuelta
    if (setting_fade_auto(channel, 0) != ESP_OK)
    {
        return ESP_FAIL;
    }
    vTaskDelay(pdMS_TO_TICKS(1000 + 10));

    return ESP_OK;
}