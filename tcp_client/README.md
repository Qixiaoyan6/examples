| Supported Targets | ESP32 | ESP32-C2 | ESP32-C3 | ESP32-S2 | ESP32-S3 |
| ----------------- | ----- | -------- | -------- | -------- | -------- |


# TCP Client 示例

应用程序创建一个TCP客户端，并尝试使用预定义的IP地址和端口号连接到服务器。当连接成功建立时，应用程序发送消息并等待应答。服务器回复后，应用程序将收到的回复打印为ASCII文本，等待2秒后再发送另一条消息。

## 如何使用示例

为了创建与TCP客户端通信的TCP服务器，请选择以下选项之一。

有许多主机端工具可用于与UDP/TCP服务器/客户端进行交互。

一个命令行工具是[netcat](http://netcat.sourceforge.net)，它可以发送和接收多种数据包。

注意:请将“192.168.0.167 3333”替换为所需的IPV4/IPV6地址(显示在监控控制台)和端口号。

除了这些工具之外，还可以在sockets/scripts目录下找到简单的Python脚本。每个脚本都设计为与其中一个示例交互。

### Python脚本
脚本example_test.py可以用作tcp-client项目的对应部分，ip协议名称(IPv4或IPv6)应作为参数声明。例子:

```
python example_test.py IPv4
```
请注意，该脚本也用于自动化测试，因此需要导入IDF测试框架包;

请将` $IDF_PATH/tools/ci/python_packages `添加到` PYTHONPATH`。

## 硬件要求

本例可以在任何常用的ESP32开发板上运行。

## 工程配置

```
idf.py menuconfig
```

在“示例配置选项”中设置以下参数:

* Set `IP version` of example to be IPV4 or IPV6.

* Set `IPV4 Address` in case your chose IP version IPV4 above.

* Set `IPV6 Address` in case your chose IP version IPV6 above.

* Set `Port` 表示示例将连接到的远程端口

在“连接配置示例”菜单下配置Wi-Fi或以太网。参见[examples/protocols/README.md](../../README.md)中的“建立Wi-Fi或以太网连接”一节了解更多细节。

## 编译和下载

构建项目并将其flash到板上，然后运行monitor工具查看串行输出:

```
idf.py -p PORT flash monitor
```

(如果需要退出monitor, 输入 ``Ctrl-]``.)

有关配置和使用ESP-IDF构建项目的完整步骤，请参阅入门指南。
