#include "auto_fade.h"

esp_err_t init_ledc_fade()
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

    //Instalacion del modo fading

    ESP_ERROR_CHECK(ledc_fade_func_install(0));

    return ESP_OK;
}

esp_err_t auto_fading()
{
    esp_err_t err_set = ledc_set_fade_with_time(SPEED_LEDC, CHAN_LEDC, LIMIT_LEDC, 1000);
    if(err_set != ESP_OK)
    {
        //ESP_LOGW("DEBUG", "Error seteando el fading: %s", esp_err_to_name(err_set));
        return err_set;
    }

    esp_err_t err_start = ledc_fade_start(SPEED_LEDC, CHAN_LEDC, LEDC_FADE_NO_WAIT);
    if(err_start != ESP_OK)
    {
        //ESP_LOGW("DEBUG", "Error iniciando el fading: %s", esp_err_to_name(err_set));
        return err_start;
    }
}