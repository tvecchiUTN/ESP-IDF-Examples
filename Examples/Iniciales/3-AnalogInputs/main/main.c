#include <stdio.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "driver/gpio.h"
#include "esp_log.h"
//Libreria utilizada
#include "esp_adc/adc_oneshot.h"

static const char *TAG = "Ejemplo 3 entrada analoga";

//Funcion que crea la configuracion del potenciometro
void init_analog_in(adc_oneshot_unit_handle_t *pot_handle)
{
    //Configuracion inicial del potenciometro
    adc_oneshot_unit_init_cfg_t pot_conf = {};
    pot_conf.unit_id = ADC_UNIT_1;
    pot_conf.ulp_mode = ADC_ULP_MODE_DISABLE;
    pot_conf.clk_src = ADC_RTC_CLK_SRC_DEFAULT;

    ESP_ERROR_CHECK(adc_oneshot_new_unit(&pot_conf, pot_handle));

    //Configuracion del canal
    adc_oneshot_chan_cfg_t pot_chan = {};
    pot_chan.atten = ADC_ATTEN_DB_12;
    pot_chan.bitwidth = ADC_BITWIDTH_DEFAULT;

    ESP_ERROR_CHECK(adc_oneshot_config_channel(*pot_handle, ADC_CHANNEL_6, &pot_chan));

    ESP_LOGI(TAG, "ADC Inicializado correctamente");

}

void app_main(void)
{
    //Handle o manejador del potenciometro
    adc_oneshot_unit_handle_t pot_handle;

    //Establece la configuracion
    init_analog_in(&pot_handle);

    while(1)
    {
        //Variable que guarda los valores del potenciometro
        int entrada = 0;

        //Lee los valores recibidos
        ESP_ERROR_CHECK(adc_oneshot_read(pot_handle, ADC_CHANNEL_6, &entrada));

        printf("%d\n", entrada);

        //Delay de 100 milisegundos
        vTaskDelay(pdMS_TO_TICKS(100));
    }

    //Elimina el handle y libera recursos
    adc_oneshot_del_unit(pot_handle);
}