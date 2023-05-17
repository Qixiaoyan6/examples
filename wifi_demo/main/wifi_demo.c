/* WiFi station Example

   This example code is in the Public Domain (or CC0 licensed, at your option.)

   Unless required by applicable law or agreed to in writing, this
   software is distributed on an "AS IS" BASIS, WITHOUT WARRANTIES OR
   CONDITIONS OF ANY KIND, either express or implied.
*/
#include <string.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "freertos/event_groups.h"
#include "esp_system.h"
#include "esp_wifi.h"
#include "esp_event.h"
#include "esp_log.h"
#include "nvs_flash.h"
#include "esp_mac.h"

#include "lwip/err.h"
#include "lwip/sys.h"

#define EXAMPLE_ESP_STA_WIFI_SSID CONFIG_ESP_STA_WIFI_SSID
#define EXAMPLE_ESP_STA_WIFI_PASS CONFIG_ESP_STA_WIFI_PASSWORD
#define EXAMPLE_ESP_AP_WIFI_SSID CONFIG_ESP_AP_WIFI_SSID
#define EXAMPLE_ESP_AP_WIFI_PASS CONFIG_ESP_AP_WIFI_PASSWORD
#define EXAMPLE_ESP_AP_WIFI_CHANNEL CONFIG_ESP_AP_WIFI_CHANNEL
#define EXAMPLE_MAX_STA_CONN CONFIG_ESP_MAX_STA_CONN
#define EXAMPLE_ESP_MAXIMUM_RETRY CONFIG_ESP_MAXIMUM_RETRY

#if CONFIG_ESP_WIFI_AUTH_OPEN
#define ESP_WIFI_SCAN_AUTH_MODE_THRESHOLD WIFI_AUTH_OPEN
#elif CONFIG_ESP_WIFI_AUTH_WEP
#define ESP_WIFI_SCAN_AUTH_MODE_THRESHOLD WIFI_AUTH_WEP
#elif CONFIG_ESP_WIFI_AUTH_WPA_PSK
#define ESP_WIFI_SCAN_AUTH_MODE_THRESHOLD WIFI_AUTH_WPA_PSK
#elif CONFIG_ESP_WIFI_AUTH_WPA2_PSK
#define ESP_WIFI_SCAN_AUTH_MODE_THRESHOLD WIFI_AUTH_WPA2_PSK
#elif CONFIG_ESP_WIFI_AUTH_WPA_WPA2_PSK
#define ESP_WIFI_SCAN_AUTH_MODE_THRESHOLD WIFI_AUTH_WPA_WPA2_PSK
#elif CONFIG_ESP_WIFI_AUTH_WPA3_PSK
#define ESP_WIFI_SCAN_AUTH_MODE_THRESHOLD WIFI_AUTH_WPA3_PSK
#elif CONFIG_ESP_WIFI_AUTH_WPA2_WPA3_PSK
#define ESP_WIFI_SCAN_AUTH_MODE_THRESHOLD WIFI_AUTH_WPA2_WPA3_PSK
#elif CONFIG_ESP_WIFI_AUTH_WAPI_PSK
#define ESP_WIFI_SCAN_AUTH_MODE_THRESHOLD WIFI_AUTH_WAPI_PSK
#endif

/* FreeRTOS event group to signal when we are connected*/
static EventGroupHandle_t s_wifi_event_group;

/* The event group allows multiple bits for each event, but we only care about two events:
 * - we are connected to the AP with an IP
 * - we failed to connect after the maximum amount of retries */
#define WIFI_CONNECTED_BIT BIT0
#define WIFI_FAIL_BIT BIT1

static const char *TAG = "wifi demo";

static int s_retry_num = 0;

static void event_handler(void *arg, esp_event_base_t event_base,
                          int32_t event_id, void *event_data)
{
    // WiFi STA开启事件
    if (event_base == WIFI_EVENT && event_id == WIFI_EVENT_STA_START)
    {
        // 开始WiFi连接
        esp_wifi_connect();
    }
    // WiFi STA断连事件
    else if (event_base == WIFI_EVENT && event_id == WIFI_EVENT_STA_DISCONNECTED)
    {
        wifi_event_sta_disconnected_t *disconnected = (wifi_event_sta_disconnected_t *)event_data;
        ESP_LOGI(TAG, "STA disconnect reason %d", disconnected->reason);
        // 判断是否重连
        if (s_retry_num < EXAMPLE_ESP_MAXIMUM_RETRY)
        {
            esp_wifi_connect();
            s_retry_num++;
            ESP_LOGI(TAG, "retry to connect to the AP");
        }
        else
        // 连接失败，设置事件组
        {
            xEventGroupSetBits(s_wifi_event_group, WIFI_FAIL_BIT);
        }
        ESP_LOGI(TAG, "connect to the AP fail");
    }
    // WiFi STA成功获取IP事件，获取IP表示已经成功连接路由
    else if (event_base == IP_EVENT && event_id == IP_EVENT_STA_GOT_IP)
    {
        ip_event_got_ip_t *event = (ip_event_got_ip_t *)event_data;
        ESP_LOGI(TAG, "got ip:" IPSTR, IP2STR(&event->ip_info.ip));
        s_retry_num = 0;
        // 连接成功，设置事件组
        xEventGroupSetBits(s_wifi_event_group, WIFI_CONNECTED_BIT);
    }


    
    else if (event_id == WIFI_EVENT_AP_STACONNECTED)
    {
        wifi_event_ap_staconnected_t *event = (wifi_event_ap_staconnected_t *)event_data;
        ESP_LOGI(TAG, "station " MACSTR " join, AID=%d",
                 MAC2STR(event->mac), event->aid);
    }
    else if (event_id == WIFI_EVENT_AP_STADISCONNECTED)
    {
        wifi_event_ap_stadisconnected_t *event = (wifi_event_ap_stadisconnected_t *)event_data;
        ESP_LOGI(TAG, "station " MACSTR " leave, AID=%d",
                 MAC2STR(event->mac), event->aid);
    }
}

void wifi_init_sta(void)
{
    // 创建事件组，判断是否连接到WiFi
    s_wifi_event_group = xEventGroupCreate();
    // 初始化协议栈
    ESP_ERROR_CHECK(esp_netif_init());
    // 创建默认的事件循环
    ESP_ERROR_CHECK(esp_event_loop_create_default());
    // 创建默认的STA配置
    esp_netif_create_default_wifi_sta();
    // 初始化WiFi配置
    wifi_init_config_t cfg = WIFI_INIT_CONFIG_DEFAULT();
    // 初始化WiFi
    ESP_ERROR_CHECK(esp_wifi_init(&cfg));

    esp_event_handler_instance_t instance_any_id;
    esp_event_handler_instance_t instance_got_ip;
    // 注册WiFi事件
    ESP_ERROR_CHECK(esp_event_handler_instance_register(WIFI_EVENT,
                                                        ESP_EVENT_ANY_ID,
                                                        &event_handler,
                                                        NULL,
                                                        &instance_any_id));
    // 注册IP事件
    ESP_ERROR_CHECK(esp_event_handler_instance_register(IP_EVENT,
                                                        IP_EVENT_STA_GOT_IP,
                                                        &event_handler,
                                                        NULL,
                                                        &instance_got_ip));
    // 配置STA参数
    wifi_config_t wifi_sta_config = {
        .sta = {
            // 设置连接路由的SSID
            .ssid = EXAMPLE_ESP_STA_WIFI_SSID,
            // 设置连接路由的password
            .password = EXAMPLE_ESP_STA_WIFI_PASS,
            /*
            // 设置是否根据MAC地址连接路由
            .bssid_set = true,
            // 设置根据MAC地址连接路由,需要与bssid_set配和使用
            .bssid = mac_address,
            */
            // 设置WiFi扫描方式，全信道扫描还是快速扫描
            .scan_method = WIFI_FAST_SCAN,
            // 设置路由器认证方式
            .threshold.authmode = ESP_WIFI_SCAN_AUTH_MODE_THRESHOLD,
        },
    };
    // 配置AP参数
    wifi_config_t wifi_ap_config = {
        .ap = {
            // 设置AP SSID
            .ssid = EXAMPLE_ESP_AP_WIFI_SSID,
            // 设置AP SSID长度
            .ssid_len = strlen(EXAMPLE_ESP_AP_WIFI_SSID),
            // 设置AP信道
            .channel = EXAMPLE_ESP_AP_WIFI_CHANNEL,
            // 设置AP password
            .password = EXAMPLE_ESP_AP_WIFI_PASS,
            // 设置AP最大连接数
            .max_connection = EXAMPLE_MAX_STA_CONN,
            // 设置AP加密方式
            .authmode = WIFI_AUTH_WPA_WPA2_PSK,
            // 设置beacon间隔，默认100
            .beacon_interval = 100,
            // 设置pmf
            .pmf_cfg = {
                .required = false,
            },
        },
    };
    // 如果password为空，则把WiFi设置成开放模式
    if (strlen(EXAMPLE_ESP_AP_WIFI_PASS) == 0)
    {
        wifi_ap_config.ap.authmode = WIFI_AUTH_OPEN;
    }
    // 设置WiFI STA模式
    ESP_ERROR_CHECK(esp_wifi_set_mode(WIFI_MODE_APSTA));
    // 设置WiFi STA配置
    ESP_ERROR_CHECK(esp_wifi_set_config(WIFI_IF_STA, &wifi_sta_config));
    // 设置WiFi AP配置
    ESP_ERROR_CHECK(esp_wifi_set_config(WIFI_IF_AP, &wifi_ap_config));
    // 设置WiFI Modem Sleep功能，默认WIFI_PS_MIN_MODEM
    esp_wifi_set_ps(WIFI_PS_NONE);
    // 开启WiFi功能
    ESP_ERROR_CHECK(esp_wifi_start());

    ESP_LOGI(TAG, "wifi_init_sta finished.");

    // 接收事件组事件
    EventBits_t bits = xEventGroupWaitBits(s_wifi_event_group,
                                           WIFI_CONNECTED_BIT | WIFI_FAIL_BIT,
                                           pdFALSE,
                                           pdFALSE,
                                           portMAX_DELAY);

    // 连接WiFi成功
    if (bits & WIFI_CONNECTED_BIT)
    {
        ESP_LOGI(TAG, "connected to ap SSID:%s password:%s",
                 EXAMPLE_ESP_STA_WIFI_SSID, EXAMPLE_ESP_STA_WIFI_PASS);
    }
    // 连接WiFi失败
    else if (bits & WIFI_FAIL_BIT)
    {
        ESP_LOGI(TAG, "Failed to connect to SSID:%s, password:%s",
                 EXAMPLE_ESP_STA_WIFI_SSID, EXAMPLE_ESP_STA_WIFI_PASS);
    }
    else
    {
        ESP_LOGE(TAG, "UNEXPECTED EVENT");
    }
}

void app_main(void)
{
    // 初始化NVS
    esp_err_t ret = nvs_flash_init();
    if (ret == ESP_ERR_NVS_NO_FREE_PAGES || ret == ESP_ERR_NVS_NEW_VERSION_FOUND)
    {
        ESP_ERROR_CHECK(nvs_flash_erase());
        ret = nvs_flash_init();
    }
    ESP_ERROR_CHECK(ret);

    ESP_LOGI(TAG, "WIFI DEMO");
    wifi_init_sta();
}
