#include "app_dac_os.h"
#include "esp_log.h"

#include "driver/dac_cosine.h"

static const char *TAG = "MAIN_APP";

void app_main(void)
{
    dac_oneshot_handle_t handle_dac;

    esp_err_t err_dac = init_dac_os(DAC_CHAN_0, &handle_dac);

    esp_err_t err_output;
    uint16_t outVolt;

    while(!err_dac)
    {
        for(outVolt = 0; outVolt <= 255; outVolt++)
        {
            err_output = dac_oneshot_output_voltage(handle_dac, outVolt);
            if(err_output == ESP_ERR_INVALID_ARG)
            {
                ESP_LOGW(TAG, "Argumentos invalidos");
                outVolt = 0;
            }
            vTaskDelay(1);
        }

        vTaskDelay(pdMS_TO_TICKS(1000));
        //ESP_LOGI("DEBUG", "Iniciando de 0");
    }

}