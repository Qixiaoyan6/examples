| Supported Targets | ESP32 | ESP32-S2 | ESP32-S3 | ESP32-C3 |
| ----------------- | ----- | -------- | -------- | -------- |

# Web 配网

该工程与smart_config配网方式类似，用手机/电脑等设备连接到目标单片机的ap，随后会自动跳入一个配网界面，在其中输入sta的ssid和pwd就能进行配网，配网成功的标志是在串口监视器中可以发现被连接目标一直在接收连接成功到sta中的数据。

## 如何使用示例

本示例基于esp-idf-v5.0开发，在项目配置和构建之前，请确保使用idf环境与示例环境一致，否则将出现无法编译的情况；
请确保您选择开发板设置正确' idf.py set-target <chip_name> '

### 硬件要求

*  ESP32/ESP32-S2/ESP32-C3 SoC (e.g., ESP32-DevKitC, ESP-WROVER-KIT, etc.)
* USB线，用于供电和编程
* WiFi

### 配置项目

打开工程的配置选项 (`idf.py menuconfig`).

进入改菜单内进行配置 `Example Configuration` ：

* 设置Wi-Fi配置。
    * 设置`SoftAP SSID`
    * 设置SoftAP Password`
    * 设置`Maximal STA connections`

### 编译和下载

构建项目并将其flash到板上，然后运行monitor工具查看串行输出:

```
idf.py -p PORT flash monitor
```

(如果需要退出monitor, 输入 ``Ctrl-]``.)

有关配置和使用ESP-IDF构建项目的完整步骤，请参阅入门指南。

## 示例输出：

```
I (733) example: Set up softAP with IP: 192.168.4.1
I (743) example: wifi_init_softap finished. SSID:'esp32_ssid' password:'esp32_pwd'
I (753) example: Starting server on port: '80'
I (753) example: Registering URI handlers
I (763) example_dns_redirect_server: Socket created
I (763) example_dns_redirect_server: Socket bound, port 53
I (773) example_dns_redirect_server: Waiting for data
I (1873) wifi:new:<1,1>, old:<1,1>, ap:<1,1>, sta:<255,255>, prof:1
I (1873) wifi:station: e8:84:a5:18:8f:80 join, AID=1, bgn, 40U
I (2203) example: station e8:84:a5:18:8f:80 join, AID=1
I (2833) example_dns_redirect_server: Received 50 bytes from 192.168.4.2 | DNS reply with len: 66
I (2843) example_dns_redirect_server: Waiting for data
I (3043) example_dns_redirect_server: Received 39 bytes from 192.168.4.2 | DNS reply with len: 55
I (3043) example_dns_redirect_server: Waiting for data
I (3043) example_dns_redirect_server: Received 42 bytes from 192.168.4.2 | DNS reply with len: 58
I (3053) example_dns_redirect_server: Waiting for data
W (3203) wifi:<ba-add>idx:4 (ifx:1, e8:84:a5:18:8f:80), tid:0, ssn:9, winSize:64
I (3533) example: Redirecting to root
I (5693) example_dns_redirect_server: Received 37 bytes from 192.168.4.2 | DNS reply with len: 53
I (5693) example_dns_redirect_server: Waiting for data
I (5783) example_dns_redirect_server: Received 46 bytes from 192.168.4.2 | DNS reply with len: 62
I (5783) example_dns_redirect_server: Waiting for data
I (6303) example_dns_redirect_server: Received 41 bytes from 192.168.4.2 | DNS reply with len: 57
I (6303) example_dns_redirect_server: Waiting for data
I (6303) example_dns_redirect_server: Received 41 bytes from 192.168.4.2 | DNS reply with len: 57
I (6313) example_dns_redirect_server: Waiting for data
I (6593) example: Redirecting to root
I (9623) example: Redirecting to root
I (12913) example: Redirecting to root
I (13263) example_dns_redirect_server: Received 34 bytes from 192.168.4.2 | DNS reply with len: 50
I (13273) example_dns_redirect_server: Waiting for data
I (13273) example_dns_redirect_server: Received 34 bytes from 192.168.4.2 | DNS reply with len: 50
I (13283) example_dns_redirect_server: Waiting for data
I (16303) example_dns_redirect_server: Received 32 bytes from 192.168.4.2 | DNS reply with len: 48
I (16303) example_dns_redirect_server: Waiting for data
I (18073) example: Redirecting to root
I (18273) example_dns_redirect_server: Received 34 bytes from 192.168.4.2 | DNS reply with len: 50
I (18273) example_dns_redirect_server: Waiting for data
I (18273) example_dns_redirect_server: Received 34 bytes from 192.168.4.2 | DNS reply with len: 50
I (18283) example_dns_redirect_server: Waiting for data
I (20683) example_dns_redirect_server: Received 42 bytes from 192.168.4.2 | DNS reply with len: 58
I (20683) example_dns_redirect_server: Waiting for data
I (20753) example: Redirecting to root
I (21323) example: Redirecting to root
I (22683) example_dns_redirect_server: Received 48 bytes from 192.168.4.2 | DNS reply with len: 64
I (22693) example_dns_redirect_server: Waiting for data
I (23443) example_dns_redirect_server: Received 48 bytes from 192.168.4.2 | DNS reply with len: 64
I (23453) example_dns_redirect_server: Waiting for data
I (23473) example: Serve root
I (23503) example_dns_redirect_server: Received 48 bytes from 192.168.4.2 | DNS reply with len: 64
I (23513) example_dns_redirect_server: Waiting for data
```
