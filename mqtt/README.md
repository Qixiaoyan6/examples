| Supported Targets | ESP32 | ESP32-C3 | ESP32-S2 | ESP32-S3 |
| ----------------- | ----- | -------- | -------- | -------- |

# ESP-MQTT 示例
本示例用于对接mqtt，在代码中的位置（如图）写入对应的mqtt参数，即可对接mqtt。

![image-20230516110453300](C:\Users\34088\AppData\Roaming\Typora\typora-user-images\image-20230516110453300.png)

## 怎么使用示例

### 硬件要求

本例可以在任何ESP32板上执行，唯一需要的接口是WiFi和连接到互联网。

### 配置工程

* 打开项目配置菜单 (`idf.py menuconfig`)
* 在“连接配置示例”菜单下配置Wi-Fi或以太网（`Example Connection Configuration`）
  * `WiFi SSID`
  * `WiFi Password`


### 编译和下载

构建项目并将其flash到板上，然后运行monitor工具查看串行输出:

```
idf.py -p PORT flash monitor
```

(如果需要退出monitor, 输入 ``Ctrl-]``.)

有关配置和使用ESP-IDF构建项目的完整步骤，请参阅入门指南。

## Example Output

```
I (30) boot: ESP-IDF v5.0-dirty 2nd stage bootloader
I (30) boot: compile time 14:08:07
I (30) boot: chip revision: v0.3
I (32) boot.esp32c3: SPI Speed      : 80MHz
I (37) boot.esp32c3: SPI Mode       : DIO
I (42) boot.esp32c3: SPI Flash Size : 2MB
I (47) boot: Enabling RNG early entropy source...
I (52) boot: Partition Table:
I (56) boot: ## Label            Usage          Type ST Offset   Length
I (63) boot:  0 nvs              WiFi data        01 02 00009000 00006000
I (70) boot:  1 phy_init         RF data          01 01 0000f000 00001000
I (78) boot:  2 factory          factory app      00 00 00010000 00100000
I (85) boot: End of partition table
I (90) esp_image: segment 0: paddr=00010020 vaddr=3c0a0020 size=23598h (144792) map
I (121) esp_image: segment 1: paddr=000335c0 vaddr=3fc90000 size=02bbch ( 11196) load
I (123) esp_image: segment 2: paddr=00036184 vaddr=40380000 size=09e94h ( 40596) load
I (134) esp_image: segment 3: paddr=00040020 vaddr=42000020 size=9f0a0h (651424) map
I (237) esp_image: segment 4: paddr=000df0c8 vaddr=40389e94 size=0605ch ( 24668) load
I (241) esp_image: segment 5: paddr=000e512c vaddr=50000010 size=00010h (    16) load
I (247) boot: Loaded app from partition at offset 0x10000
I (248) boot: Disabling RNG early entropy source...
I (265) cpu_start: Pro cpu up.
I (274) cpu_start: Pro cpu start user code
I (274) cpu_start: cpu freq: 160000000 Hz
I (274) cpu_start: Application information:
I (277) cpu_start: Project name:     mqtt_tcp
I (282) cpu_start: App version:      1
I (286) cpu_start: Compile time:     May 16 2023 14:07:35
I (293) cpu_start: ELF file SHA256:  49bbbf83910bd954...
I (299) cpu_start: ESP-IDF:          v5.0-dirty
I (304) heap_init: Initializing. RAM available for dynamic allocation:
I (311) heap_init: At 3FC96E50 len 000458C0 (278 KiB): DRAM
I (2921) wifi:new:<6,0>, old:<1,0>, ap:<255,255>, sta:<6,0>, prof:1
I (3571) wifi:state: init -> auth (b0)
I (4571) wifi:state: auth -> init (200)
I (4571) wifi:new:<6,0>, old:<6,0>, ap:<255,255>, sta:<6,0>, prof:1
I (4571) example_connect: Wi-Fi disconnected, trying to reconnect...
I (6981) example_connect: Wi-Fi disconnected, trying to reconnect...
I (9391) wifi:new:<6,0>, old:<6,0>, ap:<255,255>, sta:<6,0>, prof:1
I (9391) wifi:state: init -> auth (b0)
I (9411) wifi:state: auth -> assoc (0)
I (9421) wifi:state: assoc -> run (10)
I (9441) wifi:connected with WT, aid = 21, channel 6, BW20, bssid = 6c:06:d6:11:8c:78
I (9441) wifi:security: WPA2-PSK, phy: bgn, rssi: -43
I (9441) wifi:pm start, type: 1

I (9441) wifi:set rx beacon pti, rx_bcn_pti: 0, bcn_timeout: 0, mt_pti: 25000, mt_time: 10000
I (9531) wifi:BcnInt:102400, DTIM:1
I (10361) esp_netif_handlers: example_netif_sta ip: 192.169.3.232, mask: 255.255.255.0, gw: 192.169.3.1
I (10361) example_connect: Got IPv4 event: Interface "example_netif_sta" address: 192.169.3.232
I (11361) example_connect: Got IPv6 event: Interface "example_netif_sta" address: fe80:0000:0000:0000:6255:f9ff:fe76:5358, type: ESP_IP6_ADDR_IS_LINK_LOCAL
I (11361) example_common: Connected to example_netif_sta
I (11371) example_common: - IPv4 address: 192.169.3.232,
I (11371) example_common: - IPv6 address: fe80:0000:0000:0000:6255:f9ff:fe76:5358, type: ESP_IP6_ADDR_IS_LINK_LOCAL
I (11381) MQTT_EXAMPLE: Other event id:7
W (11581) wifi:<ba-add>idx:0 (ifx:0, 6c:06:d6:11:8c:78), tid:6, ssn:1, winSize:64
I (11601) MQTT_EXAMPLE: MQTT_EVENT_CONNECTED
I (11611) MQTT_EXAMPLE: sent publish successful, msg_id=53315
I (11611) MQTT_EXAMPLE: sent subscribe successful, msg_id=4871
I (11611) MQTT_EXAMPLE: sent subscribe successful, msg_id=23199
I (11621) MQTT_EXAMPLE: sent unsubscribe successful, msg_id=64467
I (11821) MQTT_EXAMPLE: MQTT_EVENT_SUBSCRIBED, msg_id=4871
I (11831) MQTT_EXAMPLE: sent publish successful, msg_id=0
I (11831) MQTT_EXAMPLE: MQTT_EVENT_SUBSCRIBED, msg_id=23199
I (11831) MQTT_EXAMPLE: sent publish successful, msg_id=0
I (11841) MQTT_EXAMPLE: MQTT_EVENT_UNSUBSCRIBED, msg_id=64467
W (26091) wifi:<ba-add>idx:1 (ifx:0, 6c:06:d6:11:8c:78), tid:0, ssn:1, winSize:64
```
