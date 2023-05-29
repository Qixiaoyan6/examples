| Supported Targets | ESP32 | ESP32-C2 | ESP32-C3 | ESP32-S2 | ESP32-S3 |
| ----------------- | ----- | -------- | -------- | -------- | -------- |

# smartconfig 示例

这个例子展示了ESP32如何通过ESPTOUCH连接到目标AP。

## 如何使用示例

本示例基于esp-idf-v5.0开发，在项目配置和构建之前，请确保使用idf环境与示例环境一致，否则将出现无法编译的情况；
请确保您选择开发板设置正确' idf.py set-target <chip_name> '

### 硬件要求

- 从APP store下载ESPTOUCH APP:

[Android source code](https://github.com/EspressifApp/EsptouchForAndroid)
[iOS source code](https://github.com/EspressifApp/EsptouchForIOS) 

### 工程配置

```
idf.py menuconfig
```

### 编译和下载

构建项目并将其flash到板上，然后运行monitor工具查看串行输出:

```
idf.py -p PORT flash monitor
```

(如果需要退出monitor, 输入 ``Ctrl-]``.)

有关配置和使用ESP-IDF构建项目的完整步骤，请参阅入门指南。

## 示例输出：

* 确保您的手机连接到目标AP (2.4GHz)。
* 打开ESPTOUCH应用程序并输入密码。几秒钟后会有成功消息。

下面是一个smartconfig控制台输出的示例。
```
I (372) wifi: mode : sta (24:0a:c4:00:44:86)
I (422) smartconfig: SC version: V2.6.6
I (3802) wifi: ic_enable_sniffer
I (3802) sc: SC_STATUS_FIND_CHANNEL
I (234592) smartconfig: TYPE: ESPTOUCH
I (234592) smartconfig: T|PHONE MAC:68:3e:34:88:59:bf
I (234592) smartconfig: T|AP MAC:a4:56:02:47:30:07
I (234592) sc: SC_STATUS_GETTING_SSID_PSWD
I (239922) smartconfig: T|pswd: 123456789
I (239922) smartconfig: T|ssid: IOT_DEMO_TEST
I (239922) smartconfig: T|bssid: a4:56:02:47:30:07
I (239922) wifi: ic_disable_sniffer
I (239922) sc: SC_STATUS_LINK
I (239932) sc: SSID:IOT_DEMO_TEST
I (239932) sc: PASSWORD:123456789
I (240062) wifi: n:1 0, o:1 0, ap:255 255, sta:1 0, prof:1
I (241042) wifi: state: init -> auth (b0)
I (241042) wifi: state: auth -> assoc (0)
I (241052) wifi: state: assoc -> run (10)
I (241102) wifi: connected with IOT_DEMO_TEST, channel 1
I (244892) event: ip: 192.168.0.152, mask: 255.255.255.0, gw: 192.168.0.1
I (244892) sc: WiFi Connected to ap
I (247952) sc: SC_STATUS_LINK_OVER
I (247952) sc: Phone ip: 192.168.0.31
I (247952) sc: smartconfig over
```
