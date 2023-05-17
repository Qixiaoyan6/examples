#include <string.h>
#include <stdio.h>
#include "esp_wifi.h"
#include "esp_log.h"

static const char *TAG = "example";


void wifi_init_sta(char* ssid,char* pwd)
{
    wifi_config_t wifi_config = {0};
    memcpy(wifi_config.sta.ssid,ssid,strlen(ssid));
    memcpy(wifi_config.sta.password,pwd,strlen(pwd));
    ESP_ERROR_CHECK(esp_wifi_set_mode(WIFI_MODE_APSTA));
    ESP_ERROR_CHECK(esp_wifi_set_config(WIFI_IF_STA, &wifi_config) );//配置
    
    ESP_LOGI(TAG, "wifi_init_sta finished.");

}