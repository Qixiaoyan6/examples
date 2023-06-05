| Supported Targets | ESP32 | ESP32-C2 | ESP32-C3 | ESP32-H2 | ESP32-S3 |
| ----------------- | ----- | -------- | -------- | -------- | -------- |

# ESP-IDF Gatt Server 示例

这个例子展示了如何通过一个一个地添加属性来创建一个 GATT 服务。但是这个方法是Bluedroid自己定义的，用户使用起来比较困难。

因此，我们也允许用户创建带有属性表的 GATT 服务，从而使用户无需逐一添加属性。并且推荐用户使用。有关此方法的更多信息，请参阅：[gatt_server_service_table_demo](https://github.com/espressif/esp-idf/blob/master/examples/bluetooth/bluedroid/ble/gatt_server_service_table)。

此演示创建 GATT 服务，然后开始广播，等待连接到 GATT 客户端。

为了测试这个演示，我们可以运行 gatt_client_demo, 它可以自动扫描并连接到这个演示。一旦 GATT 客户端启用了 GATT 服务器的通知功能，它们就会开始交换数据。

请查看本[教程](https://github.com/espressif/esp-idf/blob/master/examples/bluetooth/bluedroid/ble/gatt_server/tutorial/Gatt_Server_Example_Walkthrough.md)以获取有关此示例的更多信息。

## 如何使用示例

在项目配置和构建之前，请务必使用以下方法设置正确的芯片目标：

```bash
idf.py set-target <chip_name>
```

### 硬件需求

* ESP32/ESP32-C3/ESP32-C2/ESP32-H2/ESP-S3 SoC开发板（如ESP32-DevKitC、ESP-WROVER-KIT等）
* 用于供电和编程的 USB 电缆

有关它的更多信息，请参见[开发板。](https://www.espressif.com/en/products/devkits) 

### 编译和下载

运行`idf.py -p PORT flash monitor`以编译、下载和监控项目。

（要退出串行监视器，请键入`Ctrl-]`。）

有关配置和使用 ESP-IDF 构建项目的完整步骤，请参阅[入门指南。](https://idf.espressif.com/) 

### UUID128 的设置

此示例默认使用 UUID16。要更改为 UUID128，请按照以下步骤操作：

1. 将 UIID16 更改为 UUID128。您可以根据需要更改 UUID。

```c
// Create a new UUID128 (using random values for this example)
uint8_t gatts_service_uuid128_test_X[ESP_UUID_LEN_128] = {0x06, 0x18, 0x7a, 0xec, 0xbe, 0x11, 0x11, 0xea, 0x00, 0x16, 0x02, 0x42, 0x01, 0x13, 0x00, 0x04};
```

通过添加这个新的 UUID128，您可以删除`#define`具有旧 UUID16 的宏。

2. 将新的 UUID128 添加到配置文件中。

```c
// Change the size of the UUID from 16 to 128
gl_profile_tab[PROFILE_X_APP_ID].service_id.id.uuid.len = ESP_UUID_LEN_128;
// Copy the new UUID128 to the profile
memcpy(gl_profile_tab[PROFILE_X_APP_ID].service_id.id.uuid.uuid.uuid128, gatts_service_uuid128_test_X, ESP_UUID_LEN_128);
```

3. 删除以下行
```c
gl_profile_tab[PROFILE_X_APP_ID].service_id.id.uuid.uuid.uuid16 = GATTS_SERVICE_UUID_TEST_X;
```

## 示例输出

```
I (0) cpu_start: Starting scheduler on APP CPU.
I (512) BTDM_INIT: BT controller compile version [1342a48]
I (522) system_api: Base MAC address is not set
I (522) system_api: read default base MAC address from EFUSE
I (522) phy_init: phy_version 4670,719f9f6,Feb 18 2021,17:07:07
I (922) GATTS_DEMO: REGISTER_APP_EVT, status 0, app_id 0

I (932) GATTS_DEMO: CREATE_SERVICE_EVT, status 0,  service_handle 40

I (942) GATTS_DEMO: REGISTER_APP_EVT, status 0, app_id 1

I (952) GATTS_DEMO: SERVICE_START_EVT, status 0, service_handle 40

I (952) GATTS_DEMO: ADD_CHAR_EVT, status 0,  attr_handle 42, service_handle 40

I (962) GATTS_DEMO: the gatts demo char length = 3

I (962) GATTS_DEMO: prf_char[0] =11

I (972) GATTS_DEMO: prf_char[1] =22

I (972) GATTS_DEMO: prf_char[2] =33

I (982) GATTS_DEMO: CREATE_SERVICE_EVT, status 0,  service_handle 44

I (982) GATTS_DEMO: ADD_DESCR_EVT, status 0, attr_handle 43, service_handle 40

I (992) GATTS_DEMO: SERVICE_START_EVT, status 0, service_handle 44

I (1002) GATTS_DEMO: ADD_CHAR_EVT, status 0,  attr_handle 46, service_handle 44

I (1012) GATTS_DEMO: ADD_DESCR_EVT, status 0, attr_handle 47, service_handle 44

```

## 故障排除

如有任何技术疑问，请在 GitHub 上提出[问题](https://github.com/espressif/esp-idf/issues)。我们会尽快回复您。
