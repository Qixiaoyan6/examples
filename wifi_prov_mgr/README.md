| Supported Targets | ESP32 | ESP32-C2 | ESP32-C3 | ESP32-S2 | ESP32-S3 |
| ----------------- | ----- | -------- | -------- | -------- | -------- |

# Wi-Fi Provisioning Manager 示例

对于此示例，选择 BLE 作为默认传输模式，与预配相关的通信将通过该传输模式进行。NimBLE 已配置为默认主机，但您也可以使用 menuconfig -> Components -> Bluetooth -> Bluetooth Host 切换到 Bluedroid。

> 注意：由于 ESP32-S2 不支持 BLE，在这种情况下，SoftAP 将是默认的传输方式。即使对于 ESP32，您也可以从菜单配置更改为 SoftAP 传输。

在预配过程中，设备配置为具有指定凭据的 Wi-Fi 站。配置完成后，设备将保留 Wi-Fi 配置，直到执行闪存擦除。

置备完成后，BLE 将立即关闭并禁用，以释放 BLE 堆栈使用的内存。虽然， 这是特定于此示例的， 用户可以选择在自己的应用程序中保持 BLE 堆栈完好无损.

`wifi_prov_mgr`使用以下组件：

* `wifi_provisioning`：为 Wi-Fi 配置提供管理器、数据结构和原始通信端点处理程序
* `protocomm`：用于基于协议的通信和安全会话建立
* `protobuf`：谷歌的协议缓冲区库，用于原型通信数据结构的序列化
* `bt`： ESP32 BLE 堆栈，用于传输 protobuf 数据包

此示例可用于将预配服务添加到用于 IoT 的任何应用程序。

> 注意：如果您在自己的项目中使用此示例代码，在BLE模式下，请记住在SDK配置中启用BT堆栈和BTDM BLE控制设置（例如，使用此项目中的文件）。`sdkconfig.defaults`

## 如何使用示例

### 所需硬件

示例应该能够在任何常用的 ESP32/ESP32-S2 开发板上运行。

#### 平台 ： 安卓

对于 Android，GitHub 上提供了配置应用程序以及源代码: [esp-idf-provisioning-android](https://github.com/espressif/esp-idf-provisioning-android)

#### 平台 ： 苹果

对于 iOS，GitHub 上提供了配置应用程序以及源代码 : [esp-idf-provisioning-ios](https://github.com/espressif/esp-idf-provisioning-ios)

### 配置项目

```
idf.py menuconfig
```
* 在“示例配置”选项下设置 BLE/软 AP 传输。ESP32-S2 将只有 SoftAP 选项。

### 构建和刷写

编译项目并将其烧录到开发板上，然后运行监视器工具查看串行输出：

```
idf.py -p PORT flash monitor
```

（要退出串行监视器，请键入 。`Ctrl-]`

有关配置和使用 ESP-IDF 构建项目的完整步骤，请参阅入门指南。

## 示例输出

```
I (445) app: Starting provisioning
I (1035) app: Provisioning started
I (1045) wifi_prov_mgr: Provisioning started with service name : PROV_261FCC
```

确保记下串行监视器日志中显示的BLE设备名称（以`PROV_`开头）（例如。PROV_261FCC）。这将取决于 MAC ID，并且对于每个设备都是唯一的。

在一个单独的终端运行` $IDP_PATH/tools/esp_prov `目录下的` esp_prove .py `脚本(确保将'`myssid `和` mypassword `替换为设备在配置后应该连接的AP)。假设默认的示例配置，它使用基于pop(所有权证明)身份验证的协议通信安全方案 1:

```
python esp_prov.py --transport ble --service_name PROV_261FCC --sec_ver 1 --pop abcd1234 --ssid myssid --passphrase mypassword
```

对于安全版本2，可以使用以下命令:
```
python esp_prov.py --transport ble --service_name PROV_261FCC --sec_ver 2 --sec2_username wifiprov --sec2_pwd abcd1234 --ssid myssid --passphrase mypassword
```

上面的命令将执行配置步骤，监视器日志应该显示如下所示:

```
I (39725) app: Received Wi-Fi credentials
    SSID     : myssid
    Password : mypassword
.
.
.
I (45335) esp_netif_handlers: sta ip: 192.168.43.243, mask: 255.255.255.0, gw: 192.168.43.1
I (45345) app: Provisioning successful
I (45345) app: Connected with IP Address:192.168.43.243
I (46355) app: Hello World!
I (47355) app: Hello World!
I (48355) app: Hello World!
I (49355) app: Hello World!
.
.
.
I (52315) wifi_prov_mgr: Provisioning stopped
.
.
.
I (52355) app: Hello World!
I (53355) app: Hello World!
I (54355) app: Hello World!
I (55355) app: Hello World!
```

**注意:**为设备端生成安全版本2 ('`SRP6a `和验证器)的凭据，可以使用以下示例命令。然后可以直接在本例中使用输出。

应该为示例启用配置选项' CONFIG_EXAMPLE_PROV_SEC2_DEV_MODE '，并且在' main/app_main.c '中，宏' EXAMPLE_PROV_SEC2_USERNAME '应该设置为生成验证器时使用的相同用户名。

```log
$ python esp_prov.py --transport softap --sec_ver 2 --sec2_gen_cred --sec2_username wifiprov --sec2_pwd abcd1234
==== Salt-verifier for security scheme 2 (SRP6a) ====
static const char sec2_salt[] = {
    0x03, 0x6e, 0xe0, 0xc7, 0xbc, 0xb9, 0xed, 0xa8, 0x4c, 0x9e, 0xac, 0x97, 0xd9, 0x3d, 0xec, 0xf4
};

static const char sec2_verifier[] = {
    0x7c, 0x7c, 0x85, 0x47, 0x65, 0x08, 0x94, 0x6d, 0xd6, 0x36, 0xaf, 0x37, 0xd7, 0xe8, 0x91, 0x43,
    0x78, 0xcf, 0xfd, 0x61, 0x6c, 0x59, 0xd2, 0xf8, 0x39, 0x08, 0x12, 0x72, 0x38, 0xde, 0x9e, 0x24,
    .
    .
    .
    0xe6, 0xf6, 0x53, 0xc8, 0x31, 0xa8, 0x78, 0xde, 0x50, 0x40, 0xf7, 0x62, 0xde, 0x36, 0xb2, 0xba
};

```

### 二维码扫描

启用将在串行终端上显示一个二维码，可以从 ESP 预配手机应用程序扫描该 二维码以完成配网。

监控日志应显示如下内容：

```
I (1462) app: Provisioning started
I (1472) app: Scan this QR code from the provisioning application for Provisioning.
I (1472) QRCODE: Encoding below text with ECC LVL 0 & QR Code Version 10
I (1482) QRCODE: {"ver":"v1","name":"PROV_EA69FC","pop":"abcd1234","transport":"ble"}
GAP procedure initiated: advertise; disc_mode=2 adv_channel_map=0 own_addr_type=0 adv_filter_policy=0 adv_itvl_min=256 adv_itvl_max=256

  █▀▀▀▀▀█ ▀▀▀█▄█   ▀▀▄ █▄ ▀ █▀▀▀▀▀█
  █ ███ █  ▀▄█ █▄ ▀▄█ ▄██ █ █ ███ █
  █ ▀▀▀ █  ▄▀█▀▄▀ ▀█▄▀  ██  █ ▀▀▀ █
  ▀▀▀▀▀▀▀ █▄▀ █▄█▄█ ▀ █ █ ▀ ▀▀▀▀▀▀▀
  ▀▀ ▀▀ ▀  ▀▄ ▀▄ ▄▀▀▀█ ▀▄ ▀ ▀▄▄ ▄▄▀
  ███▄█▄▀ █▀  ▀▀▀▀▄▄█   █▀ █  ▄█▄█▀
  ▀███▀ ▀▄▄██ ▄▄██▄ ▀▀▀▀   ▄▀█ ▀▄▄▀
  ▄███  ▀██▀▀ ▄ ▄█▄▀▀█▄ ▀▄▀▄▄█  ▄
  ▀█▀ █▄▀▀ ▀▀█▀▀ █▀▄▀▄▀ ▄█  ███▄ ██
  ██▀█  ▀▄█ █▄▀▄███▀▄▀█ ▀█ █▀▀ ▀▄▄▀
  █▄▀▄█▀▀ ▀▄ ▀▄▄█▄▀▀█▄█▄█▀▀█ ▀▄ ▄▀
  █ ▄█▄ ▀ ▄▀ █▄  ▀█▄█▄▀▀█▀█ ▄█ ▀▄▄█
  ▀▀▀▀  ▀ █▀█▀▀▄▄██▄█▀█ ▀██▀▀▀█▄▄▀
  █▀▀▀▀▀█   ▄█▀▀▀██ ▄▀▄ █▄█ ▀ █ ▄ ▄
  █ ███ █ █ ▀▄█▀▀█▀▄█▄▄ ▀██▀▀▀▀▄▄▀▀
  █ ▀▀▀ █ ▄█ ▀ ▄█▀█ █▀ ▀▀███▄▀█ █▄█
  ▀▀▀▀▀▀▀ ▀  ▀  ▀▀ ▀     ▀▀▀▀▀▀


I (1702) app: If QR code is not visible, copy paste the below URL in a browser.
https://espressif.github.io/esp-jumpstart/qrcode.html?data={"ver":"v1","name":"PROV_EA69FC","pop":"abcd1234","transport":"ble"}
```

### 无线扫描

预配管理器还支持在发放过程中提供实时Wi-Fi扫描结果(在设备上执行)。这允许客户端应用程序选择要为其配置设备Wi-Fi站的AP。关于可见ap的各种信息是可用的，如信号强度(RSSI)和安全类型等。此外，管理器现在提供功能信息，客户端应用程序可以使用这些信息来确定特定功能的安全类型和可用性(如' wifi_scan ')。

当使用基于扫描的配置时，我们不需要显式地指定'——ssid '和'——passphrase '字段:

```
python esp_prov.py --transport ble --service_name PROV_261FCC --pop abcd1234
```

下面是' esp_prov '工具运行上述命令时的示例输出:

```
Connecting...
Connected
Getting Services...
Security scheme determined to be : 1

==== Starting Session ====
==== Session Established ====

==== Scanning Wi-Fi APs ====
++++ Scan process executed in 1.9967520237 sec
++++ Scan results : 5

++++ Scan finished in 2.7374596596 sec
==== Wi-Fi Scan results ====
S.N. SSID                              BSSID         CHN RSSI AUTH
[ 1] MyHomeWiFiAP                      788a20841996    1 -45  WPA2_PSK
[ 2] MobileHotspot                     7a8a20841996   11 -46  WPA2_PSK
[ 3] MyHomeWiFiAP                      788a208daa26   11 -54  WPA2_PSK
[ 4] NeighborsWiFiAP                   8a8a20841996    6 -61  WPA2_PSK
[ 5] InsecureWiFiAP                    dca4caf1227c    7 -74  Open

Select AP by number (0 to rescan) : 1
Enter passphrase for MyHomeWiFiAP :

==== Sending Wi-Fi Credentials to Target ====
==== Wi-Fi Credentials sent successfully ====

==== Applying Wi-Fi Config to Target ====
==== Apply config sent successfully ====

==== Wi-Fi connection state  ====
==== WiFi state: Connected ====
==== Provisioning was successful ====
```

### 发送自定义数据

预配管理器允许应用程序在预配期间发送一些自定义数据，这些数据可能是 对于其他一些操作（例如连接到某些云服务）是必需的。这是通过创建 并使用以下 API 注册其他终结点

```
wifi_prov_mgr_endpoint_create();
wifi_prov_mgr_endpoint_register();
```

在这个特定的示例中，我们添加了一个名为“custom-data”的端点，可以对其进行测试

通过将'——custom_data <MyCustomData> '选项传递给esp\_prov工具。下面的输出是

对成功的期望:

```
==== Sending Custom data to esp32 ====
CustomData response: SUCCESS
```

## 故障 排除

### 预配失败

提供的Wi-Fi凭据可能不正确，或者设备无法建立与网络的连接，其中' esp_prove '脚本将通知失败(有原因)。串行监视器日志将显示故障以及断开原因:

```
E (367015) app: Provisioning failed!
    Reason : Wi-Fi AP password incorrect
    Please reset to factory and retry provisioning
```

一旦应用了凭据，即使提供了错误的凭据，设备在随后的重启中将不再进入供应模式，直到NVS被擦除(参见以下部分)。

### 预配未启动

如果串行监视器日志显示以下内容：

```
I (465) app: Already provisioned, starting Wi-Fi STA
```

这意味着设备之前已经配置成功或没有成功(例如上节所述的场景)，或者Wi-Fi凭据已经由其他应用程序先前闪过到您的设备上。在将日志级别设置为DEBUG时，这很明显:

```
D (455) wifi_prov_mgr: Found Wi-Fi SSID     : myssid
D (465) wifi_prov_mgr: Found Wi-Fi Password : m********d
I (465) app: Already provisioned, starting Wi-Fi STA
```

要解决这个问题，我们只需要从闪存中擦除NVS分区。首先我们需要找出它的地址和大小。这可以从重启后右上方的监视器日志中看到。

```
I (47) boot: Partition Table:
I (50) boot: ## Label            Usage          Type ST Offset   Length
I (58) boot:  0 nvs              WiFi data        01 02 00009000 00006000
I (65) boot:  1 phy_init         RF data          01 01 0000f000 00001000
I (73) boot:  2 factory          factory app      00 00 00010000 00124f80
I (80) boot: End of partition table
```

现在运行以下命令擦除NVS分区:

```
$IDF_PATH/components/esptool_py/esptool/esptool.py erase_region 0x9000 0x6000
```

### 配置期间的蓝牙配对请求

ESP-IDF现在具有在对供应服务的特征执行GATT写入时强制链路加密需求的功能。但是，如果链接没有加密，这将导致配对弹出对话框。默认情况下，该特性是关闭的。为了启用此功能，请在sdkconfig中设置“CONFIG_WIFI_PROV_BLE_FORCE_ENCRYPTION=y”或使用“idf.py menuconfig”选择配置。.

```
Component Config --> Wi-Fi Provisioning Manager --> Force Link Encryption during Characteristic Read/Write

```
使用上述更改重新编译应用程序应该足以启用此功能。

### 不支持的平台

如果运行' esp_prov '的平台要求不满足，则脚本执行将回退到控制台模式，在这种情况下，整个过程(包括用户输入)将如下所示:

```
==== Esp_Prov Version: v1.0 ====
BLE client is running in console mode
    This could be due to your platform not being supported or dependencies not being met
    Please ensure all pre-requisites are met to run the full fledged client
BLECLI >> Please connect to BLE device `PROV_261FCC` manually using your tool of choice
BLECLI >> Was the device connected successfully? [y/n] y
BLECLI >> List available attributes of the connected device
BLECLI >> Is the service UUID '0000ffff-0000-1000-8000-00805f9b34fb' listed among available attributes? [y/n] y
BLECLI >> Is the characteristic UUID '0000ff53-0000-1000-8000-00805f9b34fb' listed among available attributes? [y/n] y
BLECLI >> Is the characteristic UUID '0000ff51-0000-1000-8000-00805f9b34fb' listed among available attributes? [y/n] y
BLECLI >> Is the characteristic UUID '0000ff52-0000-1000-8000-00805f9b34fb' listed among available attributes? [y/n] y

==== Verifying protocol version ====
BLECLI >> Write following data to characteristic with UUID '0000ff53-0000-1000-8000-00805f9b34fb' :
    >> 56302e31
BLECLI >> Enter data read from characteristic (in hex) :
    << 53554343455353
==== Verified protocol version successfully ====

==== Starting Session ====
BLECLI >> Write following data to characteristic with UUID '0000ff51-0000-1000-8000-00805f9b34fb' :
    >> 10015a25a201220a20ae6d9d5d1029f8c366892252d2d5a0ffa7ce1ee5829312545dd5f2aba057294d
BLECLI >> Enter data read from characteristic (in hex) :
    << 10015a390801aa0134122048008bfc365fad4753dc75912e0c764d60749cb26dd609595b6fbc72e12614031a1089733af233c7448e7d7fb7963682c6d8
BLECLI >> Write following data to characteristic with UUID '0000ff51-0000-1000-8000-00805f9b34fb' :
    >> 10015a270802b2012212204051088dc294fe4621fac934a8ea22e948fcc3e8ac458aac088ce705c65dbfb9
BLECLI >> Enter data read from characteristic (in hex) :
    << 10015a270803ba01221a20c8d38059d5206a3d92642973ac6ba8ac2f6ecf2b7a3632964eb35a0f20133adb
==== Session Established ====

==== Sending Wifi credential to esp32 ====
BLECLI >> Write following data to characteristic with UUID '0000ff52-0000-1000-8000-00805f9b34fb' :
    >> 98471ac4019a46765c28d87df8c8ae71c1ae6cfe0bc9c615bc6d2c
BLECLI >> Enter data read from characteristic (in hex) :
    << 3271f39a
==== Wifi Credentials sent successfully ====

==== Applying config to esp32 ====
BLECLI >> Write following data to characteristic with UUID '0000ff52-0000-1000-8000-00805f9b34fb' :
    >> 5355
BLECLI >> Enter data read from characteristic (in hex) :
    << 1664db24
==== Apply config sent successfully ====

==== Wifi connection state  ====
BLECLI >> Write following data to characteristic with UUID '0000ff52-0000-1000-8000-00805f9b34fb' :
    >> 290d
BLECLI >> Enter data read from characteristic (in hex) :
    << 505f72a9f8521025c1964d7789c4d7edc56aedebd144e1b667bc7c0975757b80cc091aa9f3e95b06eaefbc30290fa1
++++ WiFi state: connected ++++
==== Provisioning was successful ====
```

写入数据将从控制台输出'``>> ``复制到平台特定的应用程序，从应用程序读取的数据将粘贴到控制台的用户输入提示`` << ``，格式(十六进制)如上例日志中所示。
