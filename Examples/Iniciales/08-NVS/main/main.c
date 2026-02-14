#include <stdio.h>
#include <string.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "esp_log.h"
#include "esp_err.h"

#include "nvs_flash.h"
#include "nvs.h"

static void nvs_full_dump(const char* part_name, const char* namespace_name)
{
    if((strlen(part_name) > 15) || (strlen(namespace_name) > 15))
    {
        ESP_LOGW(TAG, "Nombres muy largos, reduzcalos");
        return;
    }

    nvs_iterator_t it = NULL;
    esp_err_t res = nvs_entry_find(part_name, namespace_name, NVS_TYPE_ANY, &it);
    while (res == ESP_OK)
    {
        nvs_entry_info_t info;
        nvs_entry_info(it, &info);
        printf("key '%s', type '%d' \n", info.key, info.type);
        res = nvs_entry_next(&it);
    }
    nvs_release_iterator(it);

    return;
}

static const char *TAG = "Ejemplo 08 - NVS";

void app_main(void)
{
    esp_err_t err_nvs = nvs_flash_init();
    if ((err_nvs == ESP_ERR_NVS_NO_FREE_PAGES) || (err_nvs == ESP_ERR_NVS_NEW_VERSION_FOUND))
    {
        ESP_ERROR_CHECK(nvs_flash_erase());
        err_nvs = nvs_flash_init();
    }

    ESP_ERROR_CHECK(err_nvs);

    ESP_LOGI(TAG, "NVS iniciado correctamente");

    nvs_handle_t handle_nvs;

    err_nvs = nvs_open("storage", NVS_READONLY, &handle_nvs);

    uint8_t ret_nvs_get;
    esp_err_t err_nvs_get = nvs_get_u8(handle_nvs, "Valor generico", &ret_nvs_get);
    if(err_nvs_get == ESP_ERR_NVS_NOT_FOUND)
    {
        esp_err_t err_nvs_set = nvs_set_u8(handle_nvs, "Valor generico", 123);
        ESP_ERROR_CHECK(err_nvs_set);
    }
    else if(err_nvs_get == ESP_OK)
    {
        ESP_LOGI(TAG, "Los datos ya esgaban guardados y su valor era %d", ret_nvs_get);
    }
    else if(err_nvs_get != ESP_OK)
    {
        ESP_LOGW(TAG, "Error guardando datos");
    }

    err_nvs = nvs_commit(handle_nvs);
    ESP_ERROR_CHECK(err_nvs);

    nvs_close(handle_nvs);

    ESP_LOGI(TAG, "NVS cerrado correctamente");

    while (1)
    {

        vTaskDelay(pdMS_TO_TICKS(1000));
    }
}
