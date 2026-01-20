#include "man_fade.h"
#include "comon.h"

esp_err_t init_ledc()
{
    ledc_timer_config_t cfg_timer_ledc = {0};
    cfg_timer_ledc.clk_cfg = LEDC_AUTO_CLK;
    cfg_timer_ledc.deconfigure = false;
    cfg_timer_ledc.duty_resolution = RES_LEDC;
    cfg_timer_ledc.freq_hz = FREC_LEDC;
    cfg_timer_ledc.speed_mode = SPEED_LEDC;
    cfg_timer_ledc.timer_num = TIMER_LEDC;

    ESP_ERROR_CHECK(ledc_timer_config(&cfg_timer_ledc));

    ledc_channel_config_t cfg_channel_ledc = {0};
    cfg_channel_ledc.channel = CHAN_LEDC;
    cfg_channel_ledc.duty = 0;
    cfg_channel_ledc.flags.output_invert = 0;
    cfg_channel_ledc.gpio_num = GPIO_LEDC;
    cfg_channel_ledc.hpoint = 0;
    cfg_channel_ledc.intr_type = LEDC_INTR_DISABLE;
    cfg_channel_ledc.sleep_mode = LEDC_SLEEP_MODE_NO_ALIVE_NO_PD;
    cfg_channel_ledc.speed_mode = SPEED_LEDC;
    cfg_channel_ledc.timer_sel = TIMER_LEDC;

    ESP_ERROR_CHECK(ledc_channel_config(&cfg_channel_ledc));

    return ESP_OK;
}

esp_err_t manual_fading()
{
    for (int i = 0; i < LIMIT_LEDC; i++)
    {
        esp_err_t err_set = ledc_set_duty(SPEED_LEDC, CHAN_LEDC, i);
        if (err_set != ESP_OK)
        {
            //ESP_LOGW(TAG, "Arguments error");
            vTaskDelay(1000);
        }

        esp_err_t err_upd = ledc_update_duty(SPEED_LEDC, CHAN_LEDC);
        if (err_upd != ESP_OK)
        {
            //ESP_LOGW(TAG, "Arguments error");
            vTaskDelay(1000);
        }
    }

    vTaskDelay(pdMS_TO_TICKS(1000));

    for (int i = LIMIT_LEDC; i > 0; i--)
    {
        esp_err_t err_set = ledc_set_duty(SPEED_LEDC, CHAN_LEDC, i);
        if (err_set != ESP_OK)
        {
            //ESP_LOGW(TAG, "Arguments error");
            vTaskDelay(1000);
        }

        esp_err_t err_upd = ledc_update_duty(SPEED_LEDC, CHAN_LEDC);
        if (err_upd != ESP_OK)
        {
            //ESP_LOGW(TAG, "Arguments error");
            vTaskDelay(1000);
        }
    }

    vTaskDelay(pdMS_TO_TICKS(1000));

    return ESP_OK;
}