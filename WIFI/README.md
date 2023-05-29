# WiFi demo示例

这个例子展示了如何使用ESP的Wi-Fi驱动程序的Wi-Fi站功能来连接到一个接入点，也演示了如何使用ESP的Wi-Fi驱动程序的Wi-Fisoft ap功能作为接入点。

## 如何使用该示例

本示例基于esp-idf-v5.0开发，在项目配置和构建之前，请确保使用idf环境与示例环境一致，否则将出现无法编译的情况；
请确保您选择开发板设置正确' idf.py set-target <chip_name> '

### 配置项目

打开项目配置菜单(`idf.py menuconfig`)。

进入` Example Configuration`:

WiFi 的配置：

设置`WiFi STA SSID`

设置`WiFi STA Password`

设置`WiFi AP SSID`

设置`WiFi AP Password`

可选:如果需要，可根据需要修改其他选项。

### 编译和下载

编译项目并将其flash到板上，然后运行monitor工具查看串行输出:

运行`idf.py -p PORT flash monitor`进行编译、下载和监视工程

(如果需要退出monitor, 输入 ``Ctrl-]``.)

有关配置和使用ESP-IDF构建项目的完整步骤，请参阅入门指南。

### 示例输出

下面是这个示例的串口监视器输出:

```c
I (30) boot: ESP-IDF v5.0-dirty 2nd stage bootloader
I (30) boot: compile time 09:44:43
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
I (90) esp_image: segment 0: paddr=00010020 vaddr=3c090020 size=1cf78h (118648) map
I (117) esp_image: segment 1: paddr=0002cfa0 vaddr=3fc90000 size=02b44h ( 11076) load
I (119) esp_image: segment 2: paddr=0002faec vaddr=40380000 size=0052ch (  1324) load
I (123) esp_image: segment 3: paddr=00030020 vaddr=42000020 size=81c04h (531460) map
I (214) esp_image: segment 4: paddr=000b1c2c vaddr=4038052c size=0f954h ( 63828) load
I (226) esp_image: segment 5: paddr=000c1588 vaddr=50000010 size=00010h (    16) load
I (231) boot: Loaded app from partition at offset 0x10000
I (231) boot: Disabling RNG early entropy source...
I (246) cpu_start: Pro cpu up.
I (255) cpu_start: Pro cpu start user code
I (255) cpu_start: cpu freq: 160000000 Hz
I (255) cpu_start: Application information:
I (258) cpu_start: Project name:     wifi_demo
I (263) cpu_start: App version:      1
I (267) cpu_start: Compile time:     May 17 2023 09:44:13
I (273) cpu_start: ELF file SHA256:  c03124577c15e8e4...
I (279) cpu_start: ESP-IDF:          v5.0-dirty
I (285) heap_init: Initializing. RAM available for dynamic allocation:
I (292) heap_init: At 3FC96C90 len 00045A80 (278 KiB): DRAM
I (298) heap_init: At 3FCDC710 len 00002950 (10 KiB): STACK/DRAM
I (305) heap_init: At 50000020 len 00001FE0 (7 KiB): RTCRAM
I (312) spi_flash: detected chip: generic
I (316) spi_flash: flash io: dio
W (320) spi_flash: Detected size(4096k) larger than the size in the binary image header(2048k). Using the size in the binary image header.
I (333) cpu_start: Starting scheduler.
I (345) wifi demo: WIFI DEMO
I (345) pp: pp rom version: 9387209
I (345) net80211: net80211 rom version: 9387209
I (365) wifi:wifi driver task: 3fc9f9f4, prio:23, stack:6656, core=0
I (365) system_api: Base MAC address is not set
I (365) system_api: read default base MAC address from EFUSE
I (375) wifi:wifi firmware version: 0d470ef
I (375) wifi:wifi certification version: v7.0
I (375) wifi:config NVS flash: enabled
I (385) wifi:config nano formating: disabled
I (385) wifi:Init data frame dynamic rx buffer num: 32
I (395) wifi:Init management frame dynamic rx buffer num: 32
I (395) wifi:Init management short buffer num: 32
I (405) wifi:Init dynamic tx buffer num: 32
I (405) wifi:Init static tx FG buffer num: 2
I (405) wifi:Init static rx buffer size: 1600
I (415) wifi:Init static rx buffer num: 10
I (415) wifi:Init dynamic rx buffer num: 32
I (425) wifi_init: rx ba win: 6
I (425) wifi_init: tcpip mbox: 32
I (425) wifi_init: udp mbox: 6
I (435) wifi_init: tcp mbox: 6
I (435) wifi_init: tcp tx win: 5744
I (435) wifi_init: tcp rx win: 5744
I (445) wifi_init: tcp mss: 1440
I (445) wifi_init: WiFi IRAM OP enabled
I (455) wifi_init: WiFi RX IRAM OP enabled
I (455) wifi:Set ps type: 0

I (465) phy_init: phy_version 912,d001756,Jun  2 2022,16:28:07
I (495) wifi:mode : sta (60:55:f9:76:53:58) + softAP (60:55:f9:76:53:59)
I (495) wifi:enable tsf
I (495) wifi:Total power save buffer number: 16
I (495) wifi:Init max length of beacon: 752/752
I (505) wifi:Init max length of beacon: 752/752
I (505) wifi demo: wifi_init_sta finished.
I (555) wifi:ap channel adjust o:1,1 n:6,2
I (555) wifi:new:<6,0>, old:<1,1>, ap:<6,2>, sta:<6,0>, prof:1
I (555) wifi:state: init -> auth (b0)
I (575) wifi:state: auth -> assoc (0)
I (585) wifi:state: assoc -> run (10)
I (605) wifi:connected with WT, aid = 5, channel 6, BW20, bssid = 6c:06:d6:11:8c:78
I (605) wifi:security: WPA2-PSK, phy: bgn, rssi: -48
I (605) wifi:pm start, type: 0
    
I (605) wifi:set rx beacon pti, rx_bcn_pti: 0, bcn_timeout: 0, mt_pti: 25000, mt_time: 10000
I (655) wifi:BcnInt:102400, DTIM:1
I (1345) esp_netif_handlers: sta ip: 192.169.3.232, mask: 255.255.255.0, gw: 192.169.3.1
I (1345) wifi demo: got ip:192.169.3.232
I (1345) wifi demo: connected to ap SSID:WT password:0987654321
I (917) phy: phy_version: 3960, 5211945, Jul 18 2018, 10:40:07, 0, 0
I (917) wifi: mode : softAP (30:ae:a4:80:45:69)
I (917) wifi softAP: wifi_init_softap finished.SSID:myssid password:mypassword
I (26457) wifi: n:1 0, o:1 0, ap:1 1, sta:255 255, prof:1
I (26457) wifi: station: 70:ef:00:43:96:67 join, AID=1, bg, 20
I (26467) wifi softAP: station:70:ef:00:43:96:67 join, AID=1
I (27657) tcpip_adapter: softAP assign IP to station,IP is: 192.168.4.2
```

