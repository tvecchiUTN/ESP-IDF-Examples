#include <stdio.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "esp_log.h"
#include "esp_err.h"

#include "nvs_flash.h" 
#include "nvs.h"

static const char* TAG = "Ejemplo 08 - NVS";

static const uint8_t* key_save[] = {"Developer", "SSID", "Password"};
static const uint8_t* value_save[] = {"VECCHI Tomas", "HUAWEI-5G-N8Cu", "tw3Xum9V"};

void app_main(void)
{
    esp_err_t err_nvs = nvs_flash_init();
    if((err_nvs == ESP_ERR_NVS_NO_FREE_PAGES) || (err_nvs == ESP_ERR_NVS_NEW_VERSION_FOUND))
    {
        ESP_ERROR_CHECK(nvs_flash_erase());
        err_nvs = nvs_flash_init();
    }

    ESP_ERROR_CHECK(err_nvs);

    nvs_handle_t handle_nvs;

    err_nvs = nvs_open("storage", NVS_READWRITE, &handle_nvs);
    ESP_ERROR_CHECK(err_nvs);

}
