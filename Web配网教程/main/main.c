/* Captive Portal Example

    This example code is in the Public Domain (or CC0 licensed, at your option.)

    Unless required by applicable law or agreed to in writing, this
    software is distributed on an "AS IS" BASIS, WITHOUT WARRANTIES OR
    CONDITIONS OF ANY KIND, either express or implied.
*/

#include <sys/param.h>

#include "esp_event.h"
#include "esp_log.h"
#include "esp_mac.h"

#include "nvs_flash.h"
#include "esp_wifi.h"
#include "esp_netif.h"
#include "lwip/inet.h"

#include "esp_http_server.h"
#include "dns_server.h"

#include "wifi_connect.h"

#define EXAMPLE_ESP_WIFI_SSID CONFIG_ESP_WIFI_SSID
#define EXAMPLE_ESP_WIFI_PASS CONFIG_ESP_WIFI_PASSWORD
#define EXAMPLE_MAX_STA_CONN CONFIG_ESP_MAX_STA_CONN

#define STA_WIFI_SSID "WT"
#define STA_WIFI_PASS "0987654321"
#define EXAMPLE_ESP_MAXIMUM_RETRY 5
#define EXAMPLE_ESP_WIFI_CHANNEL 1

extern const char root_start[] asm("_binary_root_html_start");
extern const char root_end[] asm("_binary_root_html_end");

extern const uint8_t test_jpeg_start[] asm("_binary_test_jpeg_start");
extern const uint8_t test_jpeg_end[] asm("_binary_test_jpeg_end");

static const char *TAG = "example";

static int s_retry_num = 0; /*重试次数*/

static void wifi_event_handler(void *arg, esp_event_base_t event_base,
                               int32_t event_id, void *event_data)
{
    // printf("%d %d",event_base,event_id);
    if (event_base == WIFI_EVENT && event_id == WIFI_EVENT_STA_START)
    {
        esp_wifi_connect(); /*wifi连接*/
    }
    else if (event_base == WIFI_EVENT && event_id == WIFI_EVENT_STA_DISCONNECTED)
    { /*站与AP断开连接*/
        if (s_retry_num < EXAMPLE_ESP_MAXIMUM_RETRY)
        {
            esp_wifi_connect();
            s_retry_num++;
            ESP_LOGI(TAG, "retry to connect to the AP"); /*重新尝试连接AP*/
        }
        else
        {
        }
        ESP_LOGI(TAG, "connect to the AP fail"); /*AP连接失败*/
    }
    else if (event_base == IP_EVENT && event_id == IP_EVENT_STA_GOT_IP)
    { /*工作站从连接的AP获取IP*/
        ip_event_got_ip_t *event = (ip_event_got_ip_t *)event_data;
        ESP_LOGI(TAG, "got ip:" IPSTR, IP2STR(&event->ip_info.ip));
        s_retry_num = 0;
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

static void wifi_init_softap(void)
{
    wifi_init_config_t cfg = WIFI_INIT_CONFIG_DEFAULT();
    ESP_ERROR_CHECK(esp_wifi_init(&cfg));

    ESP_ERROR_CHECK(esp_event_handler_instance_register(WIFI_EVENT,
                                                        ESP_EVENT_ANY_ID,
                                                        &wifi_event_handler,
                                                        NULL,
                                                        NULL));
    ESP_ERROR_CHECK(esp_event_handler_instance_register(IP_EVENT,
                                                        IP_EVENT_STA_GOT_IP,
                                                        &wifi_event_handler,
                                                        NULL,
                                                        NULL));

    wifi_config_t wifi_config = {
        .ap = {
            .ssid = EXAMPLE_ESP_WIFI_SSID,
            .ssid_len = strlen(EXAMPLE_ESP_WIFI_SSID),
            .password = EXAMPLE_ESP_WIFI_PASS,
            .max_connection = EXAMPLE_MAX_STA_CONN,
            .authmode = WIFI_AUTH_WPA_WPA2_PSK},
    };
    if (strlen(EXAMPLE_ESP_WIFI_PASS) == 0)
    {
        wifi_config.ap.authmode = WIFI_AUTH_OPEN;
    }

    ESP_ERROR_CHECK(esp_wifi_set_mode(WIFI_MODE_AP));
    ESP_ERROR_CHECK(esp_wifi_set_config(ESP_IF_WIFI_AP, &wifi_config));
    ESP_ERROR_CHECK(esp_wifi_start());

    esp_netif_ip_info_t ip_info; // IP 地址、网络掩码、网关 IP 地址
    esp_netif_get_ip_info(esp_netif_get_handle_from_ifkey("WIFI_AP_DEF"), &ip_info);

    char ip_addr[16];
    inet_ntoa_r(ip_info.ip.addr, ip_addr, 16); //函数功能：将网络地址转换成“.”点隔的字符串格式。
    ESP_LOGI(TAG, "Set up softAP with IP: %s", ip_addr);

    ESP_LOGI(TAG, "wifi_init_softap finished. SSID:'%s' password:'%s'",
             EXAMPLE_ESP_WIFI_SSID, EXAMPLE_ESP_WIFI_PASS);
}

// HTTP GET Handler/* URI 处理函数，在客户端发起 GET /uri 请求时被调用 */
static esp_err_t root_get_handler(httpd_req_t *req)
{
    const uint32_t root_len = root_end - root_start;

    ESP_LOGI(TAG, "Serve root");

    httpd_resp_set_type(req, "text/html");
    httpd_resp_send(req, root_start, root_len);

    return ESP_OK;
}

static esp_err_t root_post_handler(httpd_req_t *req)
{
    char buf[100] = {0};
    size_t len = req->content_len;
    int ret = httpd_req_recv(req, buf, len);
    if (ret <= 0)
    { /* 返回 0 表示连接已关闭 */
        /* 检查是否超时 */
        if (ret == HTTPD_SOCK_ERR_TIMEOUT)
        {
            /* 如果是超时，可以调用 httpd_req_recv() 重试
             * 简单起见，这里我们直接
             * 响应 HTTP 408（请求超时）错误给客户端 */
            httpd_resp_send_408(req);
        }
        /* 如果发生了错误，返回 ESP_FAIL 可以确保
         * 底层套接字被关闭 */
        return ESP_FAIL;
    }
    printf("%s\r\n", buf);
    char ssid[32] = {0};
    char pwd[64] = {0};
    /* Get value of expected key from query string */
    if (httpd_query_key_value(buf, "ssid", ssid, sizeof(ssid)) == ESP_OK)
    {
        ESP_LOGI(TAG, "Found URL query parameter => ssid=%s", ssid);
    }
    if (httpd_query_key_value(buf, "pwd", pwd, sizeof(pwd)) == ESP_OK)
    {
        ESP_LOGI(TAG, "Found URL query parameter => pwd=%s", pwd);
    }
    if (strlen(ssid) != 0 && strlen(pwd) != 0)
        wifi_init_sta(ssid, pwd);
    /* 发送简单的响应数据包 */
    const char resp[] = "WiFi connecting";
    httpd_resp_send(req, resp, HTTPD_RESP_USE_STRLEN);
    return ESP_OK;
}

static esp_err_t root_img_handler(httpd_req_t *req)
{
    httpd_resp_set_status(req, "200 OK");
    // printf("-----------------------------\r\n");
    httpd_resp_set_type(req, "image/jpeg");
    httpd_resp_send(req, (char *)test_jpeg_start, test_jpeg_end - test_jpeg_start);
    // printf("-----------------------------\r\n");
    return ESP_OK;
}

static const httpd_uri_t root = {
    .uri = "/",
    .method = HTTP_GET,
    .handler = root_get_handler,
};

static const httpd_uri_t post = {
    .uri = "/post",
    .method = HTTP_POST,
    .handler = root_post_handler,
};

static const httpd_uri_t img = {
    .uri = "/test.jpeg",
    .method = HTTP_GET,
    .handler = root_img_handler,
};

// HTTP Error (404) Handler - Redirects all requests to the root page
esp_err_t http_404_error_handler(httpd_req_t *req, httpd_err_code_t err)
{
    // Set status设置状态
    httpd_resp_set_status(req, "302 Temporary Redirect"); //临时重定向
    // Redirect to the "/" root directory重定向到“/”根目录
    httpd_resp_set_hdr(req, "Location", "http://192.168.4.1");
    // iOS requires content in the response to detect a captive portal, simply redirecting is not sufficient.
    httpd_resp_send(req, "Redirect to the captive portal", HTTPD_RESP_USE_STRLEN);

    ESP_LOGI(TAG, "Redirecting to root");
    return ESP_OK;
}

static httpd_handle_t start_webserver(void)
{
    httpd_handle_t server = NULL;                   /* 置空 esp_http_server 的实例句柄 */
    httpd_config_t config = HTTPD_DEFAULT_CONFIG(); /* 生成默认的配置参数 */
    config.max_open_sockets = 13;
    config.lru_purge_enable = true;

    // Start the httpd server/* 启动 httpd server */
    ESP_LOGI(TAG, "Starting server on port: '%d'", config.server_port);
    if (httpd_start(&server, &config) == ESP_OK)
    {
        // Set URI handlers/* 注册 URI 处理程序 */
        ESP_LOGI(TAG, "Registering URI handlers");
        httpd_register_uri_handler(server, &root);
        httpd_register_uri_handler(server, &post);
        httpd_register_uri_handler(server, &img);
        httpd_register_err_handler(server, HTTPD_404_NOT_FOUND, http_404_error_handler);
    } /* 如果服务器启动失败，返回的句柄是 NULL */
    return server;
}

void app_main(void)
{
    /*
        Turn of warnings from HTTP server as redirecting traffic will yield
        lots of invalid requests由于重定向流量将产生来自HTTP服务器的警告,大量无效请求
    */
    //设置给定标签的日志级别。
    esp_log_level_set("httpd_uri", ESP_LOG_ERROR);
    esp_log_level_set("httpd_txrx", ESP_LOG_ERROR);
    esp_log_level_set("httpd_parse", ESP_LOG_ERROR);
    // ESP_LOG_ERROR 严重错误，软件模块无法自行恢复

    // Initialize networking stack
    ESP_ERROR_CHECK(esp_netif_init());

    // Create default event loop needed by the  main app
    ESP_ERROR_CHECK(esp_event_loop_create_default());

    // Initialize NVS needed by Wi-Fi
    ESP_ERROR_CHECK(nvs_flash_init());

    // Initialize Wi-Fi including netif with default config使用默认配置初始化Wi-Fi，包括netif
    esp_netif_create_default_wifi_ap();
    esp_netif_create_default_wifi_sta();

    // Initialise ESP32 in SoftAP mode
    wifi_init_softap();

    // Start the server for the first time
    start_webserver();

    // Start the DNS server that will redirect all queries to the softAP IP
    start_dns_server(); //自动弹窗
}
