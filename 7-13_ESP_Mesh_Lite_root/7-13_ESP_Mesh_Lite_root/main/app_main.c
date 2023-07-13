/* Mesh Internal Communication Example

   This example code is in the Public Domain (or CC0 licensed, at your option.)

   Unless required by applicable law or agreed to in writing, this
   software is distributed on an "AS IS" BASIS, WITHOUT WARRANTIES OR
   CONDITIONS OF ANY KIND, either express or implied.
*/
#include "mdf_common.h"
#include "esp_wifi.h"
#include "esp_mesh_lite.h"

#include "app_wifi.h"
#include "app_rainmaker.h"
#include "app_mesh_lite.h"

static const char *TAG = "app_main";

static esp_err_t esp_storage_init(void)
{
    esp_err_t ret = nvs_flash_init();

    if (ret == ESP_ERR_NVS_NO_FREE_PAGES || ret == ESP_ERR_NVS_NEW_VERSION_FOUND) {
        // NVS partition was truncated and needs to be erased
        // Retry nvs_flash_init
        ESP_ERROR_CHECK(nvs_flash_erase());
        ret = nvs_flash_init();
    }

    return ret;
}

void session_cost_information(const char *tag, const char *func, int line, const char *desc)
{
    static uint32_t free_heap = 0;

    ESP_LOGW(tag, "%s %d %s const heap %d", func, line, desc ? desc : "NULL", esp_get_free_heap_size() - free_heap);
    free_heap = esp_get_free_heap_size();
    ESP_LOGW(tag, "free heap %d, minimum %d", free_heap, esp_get_minimum_free_heap_size());
}

void app_main(void)
{
    session_cost_information(TAG, __func__, __LINE__, "app_main");

    esp_storage_init();

    app_mesh_lite_start();

    app_rainmaker_start();

    /* Start wifi provisioning */
    app_wifi_start(POP_TYPE_RANDOM);
}

