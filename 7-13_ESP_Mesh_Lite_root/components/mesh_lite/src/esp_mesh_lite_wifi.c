// Copyright 2022 Espressif Systems (Shanghai) PTE LTD
//
// Licensed under the Apache License, Version 2.0 (the "License");
// you may not use this file except in compliance with the License.
// You may obtain a copy of the License at
//
//     http://www.apache.org/licenses/LICENSE-2.0
//
// Unless required by applicable law or agreed to in writing, software
// distributed under the License is distributed on an "AS IS" BASIS,
// WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
// See the License for the specific language governing permissions and
// limitations under the License.

#include <stdio.h>
#include <string.h>
#include "esp_log.h"
#include "esp_wifi.h"
#include "lwip/lwip_napt.h"
#include "dhcpserver/dhcpserver.h"
#include "freertos/event_groups.h"
#include "esp_mesh_lite.h"

#define SSID_MAX_LEN      (32)
#define EVENT_STA_CONNECTED   BIT0
#define ESP_ERR_DUPLICATE_ADDITION    0x110   /*!< Netif was added repeatedly */
#define DHCPS_NETIF_ID(netif) (ESP_NETIF_DHCP_SERVER & esp_netif_get_flags(netif))

#define ESP_PARAM_CHECK(con) do { \
        if (!(con)) { \
            ESP_LOGE(TAG, "<ESP_ERR_INVALID_ARG> !(%s)", #con); \
            return ESP_ERR_INVALID_ARG; \
        } \
    } while(0)

typedef esp_err_t (*dhcps_change_cb_t)(esp_ip_addr_t *ip_info);

typedef struct esp_mesh_lite_netif {
    esp_netif_t *netif;
    dhcps_change_cb_t dhcps_change_cb;
    struct esp_mesh_lite_netif *next;
} esp_mesh_lite_netif_t;

static esp_mesh_lite_netif_t *esp_mesh_lite_link = NULL;
static const char *TAG = "ESP_Mesh_Lite_Wi-Fi";
static bool esp_mesh_lite_softap_dhcps = false;
static EventGroupHandle_t s_wifi_event_group = NULL;

static esp_err_t esp_mesh_lite_netif_list_add(esp_netif_t *netif, dhcps_change_cb_t dhcps_change_cb)
{
    esp_mesh_lite_netif_t *new = esp_mesh_lite_link;
    esp_mesh_lite_netif_t *tail = NULL;

    while (new) {
        if (new->netif == netif) {
            return ESP_ERR_DUPLICATE_ADDITION;
        }

        tail = new;
        new = new->next;
    }

    // not found, create a new
    new = (esp_mesh_lite_netif_t *)malloc(sizeof(esp_mesh_lite_netif_t));

    if (new == NULL) {
        ESP_LOGE(TAG, "netif list add fail");
        return ESP_ERR_NO_MEM;
    }

    new->netif = netif;
    new->dhcps_change_cb = dhcps_change_cb;
    new->next = NULL;

    if (tail == NULL) { // the first one
        esp_mesh_lite_link = new;
    } else {
        tail->next = new;
    }

    ESP_LOGI(TAG, "netif list add success");

    return ESP_OK;
}

static bool esp_mesh_lite_netif_network_segment_is_used(uint32_t ip)
{
    esp_mesh_lite_netif_t *p = esp_mesh_lite_link;
    esp_netif_ip_info_t netif_ip = { 0 };

    while (p) {
        esp_netif_get_ip_info(p->netif, &netif_ip);

        if (esp_ip4_addr3_16((esp_ip4_addr_t *)&ip) == esp_ip4_addr3_16(&netif_ip.ip)) {
            return true;
        }

        p = p->next;
    }

    return false;
}

static esp_err_t esp_mesh_lite_netif_request_ip(esp_netif_ip_info_t *ip_info)
{
    bool ip_segment_is_used = true;

    for (uint8_t ip = 4; ip < 255; ip++) {
        ip_segment_is_used = esp_mesh_lite_netif_network_segment_is_used(ESP_IP4TOADDR(192, 168, ip, 1));
#if CONFIG_MESH_LITE_ENABLE
        ip_segment_is_used |= esp_mesh_lite_network_segment_is_used(ESP_IP4TOADDR(192, 168, ip, 1));
#endif

        if (!ip_segment_is_used) {
            ip_info->ip.addr = ESP_IP4TOADDR(192, 168, ip, 1);
            ip_info->gw.addr = ESP_IP4TOADDR(192, 168, ip, 1);
            ip_info->netmask.addr = ESP_IP4TOADDR(255, 255, 255, 0);
            ESP_LOGI("ip select", "IP Address:" IPSTR, IP2STR(&ip_info->ip));
            ESP_LOGI("ip select", "GW Address:" IPSTR, IP2STR(&ip_info->gw));
            ESP_LOGI("ip select", "NM Address:" IPSTR, IP2STR(&ip_info->netmask));

            return ESP_OK;
        }
    }

    return ESP_FAIL;
}

static esp_err_t esp_mesh_lite_netif_network_segment_conflict_update(esp_netif_t *esp_netif)
{
    esp_mesh_lite_netif_t *p = esp_mesh_lite_link;
    esp_netif_ip_info_t netif_ip;
    esp_netif_ip_info_t allocate_ip_info;
    esp_ip_addr_t esp_ip_addr_info;
    esp_ip4_addr_t netmask = {.addr = ESP_IP4TOADDR(255, 255, 255, 0)};
    bool ip_segment_is_used = false;

    memset(&allocate_ip_info, 0x0, sizeof(esp_netif_ip_info_t));

    while (p) {
        if ((esp_netif != p->netif) && DHCPS_NETIF_ID(p->netif)) { /* DHCP_Server has to be enabled for this netif */
            if (esp_netif) {
                esp_netif_get_ip_info(esp_netif, &allocate_ip_info);
            }

            esp_netif_get_ip_info(p->netif, &netif_ip);

#if CONFIG_MESH_LITE_ENABLE
            ip_segment_is_used = esp_mesh_lite_network_segment_is_used(netif_ip.ip.addr);
#endif

            /* The checked network segment does not conflict with the external netif */
            /* And the same ip net segment is not be used by other external netifs */
            if ((!ip4_addr_netcmp(&netif_ip.ip, &allocate_ip_info.ip, &netmask)) && !ip_segment_is_used) {
                p = p->next;
                continue;
            }

            if (esp_mesh_lite_netif_request_ip(&allocate_ip_info) != ESP_OK) {
                ESP_LOGE(TAG, "ip reallocate fail");
                break;
            }

            ESP_ERROR_CHECK(esp_netif_dhcps_stop(p->netif));
            esp_netif_set_ip_info(p->netif, &allocate_ip_info);
            ESP_LOGI(TAG, "ip reallocate new:" IPSTR, IP2STR(&allocate_ip_info.ip));
            ESP_ERROR_CHECK(esp_netif_dhcps_start(p->netif));

            esp_ip_addr_info.type = ESP_IPADDR_TYPE_V4;
            esp_ip_addr_info.u_addr.ip4.addr = allocate_ip_info.ip.addr;

            if (p->dhcps_change_cb) {
                p->dhcps_change_cb(&esp_ip_addr_info);
            }

            break;
        }

        p = p->next;
    }

    return ESP_OK;
}

static esp_err_t esp_mesh_lite_wifi_set(wifi_mode_t mode, const char *ssid, const char *password, const char *bssid)
{
    ESP_PARAM_CHECK(ssid);
    ESP_PARAM_CHECK(password);

    wifi_config_t wifi_cfg;
    memset(&wifi_cfg, 0x0, sizeof(wifi_config_t));

    if (mode & WIFI_MODE_STA) {
        memcpy((char *)wifi_cfg.sta.ssid, ssid, sizeof(wifi_cfg.sta.ssid));
        strlcpy((char *)wifi_cfg.sta.password, password, sizeof(wifi_cfg.sta.password));

        if (bssid != NULL) {
            wifi_cfg.sta.bssid_set = 1;
            memcpy((char *)wifi_cfg.sta.bssid, bssid, sizeof(wifi_cfg.sta.bssid));
            ESP_LOGI(TAG, "sta ssid: %s password: %s MAC "MACSTR"", ssid, password, MAC2STR(wifi_cfg.sta.bssid));
        } else {
            ESP_LOGI(TAG, "sta ssid: %s password: %s", ssid, password);
        }

        ESP_ERROR_CHECK(esp_wifi_set_config(ESP_IF_WIFI_STA, &wifi_cfg));
    }

    if (mode & WIFI_MODE_AP) {
        wifi_cfg.ap.max_connection = CONFIG_MESH_LITE_MAX_CONNECT_NUMBER;
        wifi_cfg.ap.authmode = strlen(password) < 8 ? WIFI_AUTH_OPEN : WIFI_AUTH_WPA2_PSK;
        memcpy((char *)wifi_cfg.ap.ssid, ssid, sizeof(wifi_cfg.ap.ssid));
        strlcpy((char *)wifi_cfg.ap.password, password, sizeof(wifi_cfg.ap.password));

        ESP_ERROR_CHECK(esp_wifi_set_config(ESP_IF_WIFI_AP, &wifi_cfg));

        ESP_LOGI(TAG, "softap ssid: %s password: %s", ssid, password);
    }

    return ESP_OK;
}

/* Event handler for catching system events */
static void wifi_event_sta_disconnected_handler(void *arg, esp_event_base_t event_base,
                                                int32_t event_id, void *event_data)
{
    xEventGroupClearBits(s_wifi_event_group, EVENT_STA_CONNECTED);
}

static void ip_event_sta_got_ip_handler(void *arg, esp_event_base_t event_base,
                                        int32_t event_id, void *event_data)
{
    ip_event_got_ip_t *event = (ip_event_got_ip_t *) event_data;
    ESP_LOGI(TAG, "Connected with IP Address:" IPSTR, IP2STR(&event->ip_info.ip));

    esp_mesh_lite_netif_network_segment_conflict_update(event->esp_netif);
    xEventGroupSetBits(s_wifi_event_group, EVENT_STA_CONNECTED);
}

static void wifi_event_ap_start_handler(void *arg, esp_event_base_t event_base,
                                        int32_t event_id, void *event_data)
{
    esp_netif_t* netif = esp_netif_get_handle_from_ifkey("WIFI_AP_DEF");

    if (netif) {
        if (esp_mesh_lite_softap_dhcps) {
            ESP_ERROR_CHECK(esp_netif_dhcps_stop(netif));
            esp_netif_dns_info_t dns;
            dns.ip.u_addr.ip4.addr = ESP_IP4TOADDR(114, 114, 114, 114);
            dns.ip.type = IPADDR_TYPE_V4;
            dhcps_offer_t dhcps_dns_value = OFFER_DNS;
            ESP_ERROR_CHECK(esp_netif_dhcps_option(netif, ESP_NETIF_OP_SET, ESP_NETIF_DOMAIN_NAME_SERVER, &dhcps_dns_value, sizeof(dhcps_dns_value)));
            ESP_ERROR_CHECK(esp_netif_set_dns_info(netif, ESP_NETIF_DNS_MAIN, &dns));
            ESP_ERROR_CHECK(esp_netif_dhcps_start(netif));
        }

        esp_netif_ip_info_t netif_ip;
        esp_netif_get_ip_info(netif, &netif_ip);
        ip_napt_enable(netif_ip.ip.addr, 1);
    }
}

static void wifi_event_ap_staconnected_handler(void *arg, esp_event_base_t event_base,
                                               int32_t event_id, void *event_data)
{
    ESP_LOGI(TAG, "STA Connecting to the AP again...");
}

static void wifi_event_ap_stadisconnected_handler(void *arg, esp_event_base_t event_base,
                                                  int32_t event_id, void *event_data)
{
    ESP_LOGE(TAG, "STA Disconnect to the AP");
}

static void esp_mesh_lite_event_ip_changed_handler(void *arg, esp_event_base_t event_base,
                                                  int32_t event_id, void *event_data)
{
    switch(event_id) {
        case ESP_MESH_LITE_EVENT_CORE_STARTED:
            ESP_LOGI(TAG, "Mesh-Lite connecting");
            esp_mesh_lite_connect();
            break;
        case ESP_MESH_LITE_EVENT_CORE_INHERITED_NET_SEGMENT_CHANGED:
            ESP_LOGI(TAG, "netif network segment conflict check");
            esp_mesh_lite_netif_network_segment_conflict_update(NULL);
            break;
        case ESP_MESH_LITE_EVENT_CORE_ROUTER_INFO_CHANGED:
            break;
    }
}

static esp_netif_t* create_station_netif(esp_netif_ip_info_t* ip_info, uint8_t mac[6], bool data_forwarding, bool enable_dhcps)
{
    esp_netif_t *wifi_netif = NULL;
    wifi_mode_t mode = WIFI_MODE_NULL;

    if (data_forwarding || enable_dhcps) {
        return wifi_netif;
    }

    wifi_netif = esp_netif_create_default_wifi_sta();
    esp_mesh_lite_netif_list_add(wifi_netif, NULL);

    esp_wifi_get_mode(&mode);
    mode |= WIFI_MODE_STA;
    ESP_ERROR_CHECK(esp_wifi_set_mode(mode));

    /* Register our event handler for Wi-Fi, IP and Provisioning related events */
    ESP_ERROR_CHECK(esp_event_handler_instance_register(WIFI_EVENT, WIFI_EVENT_STA_DISCONNECTED, &wifi_event_sta_disconnected_handler, NULL, NULL));
    ESP_ERROR_CHECK(esp_event_handler_instance_register(IP_EVENT, IP_EVENT_STA_GOT_IP, &ip_event_sta_got_ip_handler, NULL, NULL));

    return wifi_netif;
}

static esp_err_t softap_netif_dhcp_status_change_cb(esp_ip_addr_t *ip_info)
{
    esp_err_t ret = ESP_FAIL;
    ESP_LOGW(TAG, "SoftAP IP network segment has changed, deauth all station");
    if (esp_wifi_deauth_sta(0) == ESP_OK) {
        ret = ESP_OK;
    }

    return ret;
}

static esp_netif_t* create_softap_netif(esp_netif_ip_info_t* ip_info, uint8_t mac[6], bool data_forwarding, bool enable_dhcps)
{
    esp_netif_ip_info_t netif_ip;
    esp_netif_ip_info_t allocate_ip_info;
    char softap_ssid[SSID_MAX_LEN + 1];
    esp_netif_t *wifi_netif = NULL;
    wifi_mode_t mode = WIFI_MODE_NULL;

    if (!data_forwarding) {
        return wifi_netif;
    }

    wifi_netif = esp_netif_create_default_wifi_ap();

    if (ip_info) {
        ESP_ERROR_CHECK(esp_netif_set_ip_info(wifi_netif, ip_info));
    } else {
        if (enable_dhcps) {
            esp_mesh_lite_netif_request_ip(&allocate_ip_info);
            esp_netif_set_ip_info(wifi_netif, &allocate_ip_info);
        }
    }
    esp_netif_get_ip_info(wifi_netif, &netif_ip);
    ESP_LOGI(TAG, "IP Address:" IPSTR, IP2STR(&netif_ip.ip));
    esp_mesh_lite_netif_list_add(wifi_netif, softap_netif_dhcp_status_change_cb);

    esp_mesh_lite_softap_dhcps = enable_dhcps;

    /* Register our event handler for Wi-Fi, IP and Provisioning related events */
    ESP_ERROR_CHECK(esp_event_handler_instance_register(WIFI_EVENT, WIFI_EVENT_AP_START, &wifi_event_ap_start_handler, NULL, NULL));
    ESP_ERROR_CHECK(esp_event_handler_instance_register(WIFI_EVENT, WIFI_EVENT_AP_STACONNECTED, &wifi_event_ap_staconnected_handler, NULL, NULL));
    ESP_ERROR_CHECK(esp_event_handler_instance_register(WIFI_EVENT, WIFI_EVENT_AP_STADISCONNECTED, &wifi_event_ap_stadisconnected_handler, NULL, NULL));
    ESP_ERROR_CHECK(esp_event_handler_instance_register(ESP_MESH_LITE_EVENT, ESP_EVENT_ANY_ID, &esp_mesh_lite_event_ip_changed_handler, NULL, NULL));
    esp_wifi_get_mode(&mode);
    mode |= WIFI_MODE_AP;
    ESP_ERROR_CHECK(esp_wifi_set_mode(mode));

#if CONFIG_MESH_LITE_SOFTAP_SSID_END_WITH_THE_MAC
    uint8_t softap_mac[6];
    esp_wifi_get_mac(WIFI_IF_AP, softap_mac);
    snprintf(softap_ssid, sizeof(softap_ssid), "%.25s_%02x%02x%02x", CONFIG_MESH_LITE_SOFTAP_SSID, softap_mac[3], softap_mac[4], softap_mac[5]);
#else
    snprintf(softap_ssid, sizeof(softap_ssid), "%s", CONFIG_MESH_LITE_SOFTAP_SSID);
#endif
    esp_mesh_lite_wifi_set(WIFI_MODE_AP, softap_ssid, CONFIG_MESH_LITE_SOFTAP_PASSWORD, NULL);

    return wifi_netif;
}

void esp_mesh_lite_wifi_init(void)
{
    if (s_wifi_event_group) {
        return;
    }

    s_wifi_event_group = xEventGroupCreate();

    wifi_init_config_t cfg = WIFI_INIT_CONFIG_DEFAULT();
    ESP_ERROR_CHECK(esp_wifi_init(&cfg));

    ESP_ERROR_CHECK(esp_wifi_set_storage(WIFI_STORAGE_RAM));
    ESP_ERROR_CHECK(esp_wifi_set_mode(WIFI_MODE_NULL));

    esp_wifi_start();

    create_softap_netif(NULL, NULL, true, true);

    create_station_netif(NULL, NULL, false, false);
}