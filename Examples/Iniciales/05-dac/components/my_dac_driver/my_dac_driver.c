#include "my_dac_driver.h"
#include "esp_log.h"

static const char* ERR_TAG = "Salida de errores";

esp_err_t init_dac_os(dac_channel_t chan_id, dac_oneshot_handle_t *handle_dac_os)
{
    if(!handle_dac_os)
    {
        return ESP_ERR_INVALID_ARG;
    }

    if((chan_id != DAC_CHAN_0) && (chan_id != DAC_CHAN_1))
    {
        return ESP_ERR_INVALID_ARG;
    }

    dac_oneshot_config_t cfg_dac_os = {
        .chan_id = chan_id,
    };

    esp_err_t err_dac = dac_oneshot_new_channel(&cfg_dac_os, handle_dac_os);

    //Por las dudas
    if(err_dac == ESP_ERR_NO_MEM) //Le doy mas importancia al error de memoria insuficiente
    {
        ESP_LOGW(ERR_TAG, "Memoria insuficiente");
        return ESP_FAIL;
    }
    else if(err_dac != ESP_OK)
    {   
        ESP_LOGW(ERR_TAG, "Error al ejecutar la funcion de registro de ADC debido a: %s", esp_err_to_name(err_dac));
    }

    return err_dac;
}