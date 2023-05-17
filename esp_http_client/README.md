| Supported Targets | ESP32 | ESP32-C2 | ESP32-C3 | ESP32-S2 | ESP32-S3 |
| ----------------- | ----- | -------- | -------- | -------- | -------- |

# ESP HTTP Client 示例

本示例介绍了开发板通过何种方式去连接HTTP，访问URL中的数据

## 如何使用示例

本示例基于esp-idf-v5.0开发，在项目配置和构建之前，请确保使用idf环境与示例环境一致，否则将出现无法编译的情况；
请确保您选择开发板设置正确' idf.py set-target <chip_name> '

### 硬件要求

*  ESP32/ESP32-S2/ESP32-C3 /ESP32-S3/ESP32-C2SoC (e.g., ESP32-DevKitC, ESP-WROVER-KIT, etc.)
*  USB线，用于供电和编程

### 项目配置

打开工程的配置选项（`idf.py menuconfig`）

进入该菜单内进行配置(`Example Connection Configuration`)

* 设置`WiFi SSID`
* 设置`WiFi Password`

### 编译和下载

构建项目并将其flash到板上，然后运行monitor工具查看串行输出:

```c
idf.py -p PORT flash monitor
```

(如果需要退出monitor, 输入 ``Ctrl-]``.)

有关配置和使用ESP-IDF构建项目的完整步骤，请参阅入门指南。

### 示例输出：

```c
I (364) cpu_start: Starting scheduler.
I (373) example_connect: Start example_connect.
I (373) pp: pp rom version: 9387209
I (373) net80211: net80211 rom version: 9387209
I (393) wifi:wifi driver task: 3fc9f8a4, prio:23, stack:6656, core=0
I (393) system_api: Base MAC address is not set
I (393) system_api: read default base MAC address from EFUSE
I (403) wifi:wifi firmware version: 0d470ef
I (403) wifi:wifi certification version: v7.0
I (413) wifi:config NVS flash: enabled
I (413) wifi:config nano formating: disabled
I (413) wifi:Init data frame dynamic rx buffer num: 32
I (423) wifi:Init management frame dynamic rx buffer num: 32
I (423) wifi:Init management short buffer num: 32
I (433) wifi:Init dynamic tx buffer num: 32
I (433) wifi:Init static tx FG buffer num: 2
I (443) wifi:Init static rx buffer size: 1600
I (443) wifi:Init static rx buffer num: 10
I (443) wifi:Init dynamic rx buffer num: 32
I (453) wifi_init: rx ba win: 6
I (453) wifi_init: tcpip mbox: 32
I (453) wifi_init: udp mbox: 6
I (463) wifi_init: tcp mbox: 6
I (463) wifi_init: tcp tx win: 5744
I (473) wifi_init: tcp rx win: 5744
I (473) wifi_init: tcp mss: 1440
I (473) wifi_init: WiFi IRAM OP enabled
I (483) wifi_init: WiFi RX IRAM OP enabled
I (483) phy_init: phy_version 912,d001756,Jun  2 2022,16:28:07
I (523) wifi:mode : sta (60:55:f9:76:53:58)
I (523) wifi:enable tsf
I (533) example_connect: Connecting to WT...
I (533) example_connect: Waiting for IP(s)
I (2933) wifi:new:<6,0>, old:<1,0>, ap:<255,255>, sta:<6,0>, prof:1
I (3583) wifi:state: init -> auth (b0)
I (4583) wifi:state: auth -> init (200)
I (4593) wifi:new:<6,0>, old:<6,0>, ap:<255,255>, sta:<6,0>, prof:1
I (4593) example_connect: Wi-Fi disconnected, trying to reconnect...
I (7003) example_connect: Wi-Fi disconnected, trying to reconnect...
I (9413) wifi:new:<6,0>, old:<6,0>, ap:<255,255>, sta:<6,0>, prof:1
I (9413) wifi:state: init -> auth (b0)
I (10413) wifi:state: auth -> init (200)
I (10413) wifi:new:<6,0>, old:<6,0>, ap:<255,255>, sta:<6,0>, prof:1
I (10413) example_connect: Wi-Fi disconnected, trying to reconnect...
I (12823) example_connect: Wi-Fi disconnected, trying to reconnect...
I (15233) wifi:new:<6,0>, old:<6,0>, ap:<255,255>, sta:<6,0>, prof:1
I (15233) wifi:state: init -> auth (b0)
I (15263) wifi:state: auth -> assoc (0)
I (15273) wifi:state: assoc -> run (10)
I (15293) wifi:connected with WT, aid = 22, channel 6, BW20, bssid = 6c:06:d6:11:8c:78
I (15293) wifi:security: WPA2-PSK, phy: bgn, rssi: -51
I (15293) wifi:pm start, type: 1

I (15303) wifi:set rx beacon pti, rx_bcn_pti: 0, bcn_timeout: 0, mt_pti: 25000, mt_time: 10000
I (15363) wifi:BcnInt:102400, DTIM:1
I (16373) example_connect: Got IPv6 event: Interface "example_netif_sta" address: fe80:0000:0000:0000:6255:f9ff:fe76:5358, type: ESP_IP6_ADDR_IS_LINK_LOCAL
I (16373) esp_netif_handlers: example_netif_sta ip: 192.169.3.232, mask: 255.255.255.0, gw: 192.169.3.1
I (16383) example_connect: Got IPv4 event: Interface "example_netif_sta" address: 192.169.3.232
I (16393) example_common: Connected to example_netif_sta
I (16403) example_common: - IPv4 address: 192.169.3.232,
I (16403) example_common: - IPv6 address: fe80:0000:0000:0000:6255:f9ff:fe76:5358, type: ESP_IP6_ADDR_IS_LINK_LOCAL
I (16413) HTTP_CLIENT: Connected to AP, begin http example
W (16713) wifi:<ba-add>idx:0 (ifx:0, 6c:06:d6:11:8c:78), tid:0, ssn:1, winSize:64
I (17233) esp-x509-crt-bundle: Certificate validated
I (19273) HTTP_CLIENT: HTTP_EVENT_DISCONNECTED
I (19273) HTTP_CLIENT: HTTPS Status = 200, content_length = 12578
I (19273) HTTP_CLIENT: HTTP_EVENT_DISCONNECTED
I (19283) HTTP_CLIENT: Finish http example
```

