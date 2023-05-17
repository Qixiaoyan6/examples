| Supported Targets | ESP32 | ESP32-C3 | ESP32-S2 | ESP32-S3 |
| ----------------- | ----- | -------- | -------- | -------- |


# UDP Client 示例

应用程序创建UDP客户端，并将消息发送到预定义的端口和IP地址。在服务器回复之后，应用程序将收到的回复打印为ASCII文本，等待2秒，然后发送另一条消息。

## 如何使用该示例

为了创建与UDP客户端通信的UDP服务器，选择以下选项之一。

有许多主机端工具可用于与UDP/TCP服务器/客户端进行交互。

一个命令行工具是[netcat](http://netcat.sourceforge.net)，它可以发送和接收多种数据包。

注意:请在以下命令中将' 192.168.0.167 3333 '替换为所需的IPV4/IPV6地址(显示在监控控制台)和端口号。

除了这些工具之外，还可以在sockets/scripts目录下找到简单的Python脚本。每个脚本都设计为与其中一个示例交互。

### 通过netcat发送UDP报文
```
echo "Hello from PC" | nc -w1 -u 192.168.0.167 3333
```

### 通过netcat接收UDP报文
```
echo "Hello from PC" | nc -w1 -u 192.168.0.167 3333
```

### UDP服务器使用netcat
```
nc -u -l 192.168.0.167 3333
```

### Python脚本
脚本example_test.py可以用作udp-client应用程序的对应部分，ip协议名称(IPv4或IPv6)应作为参数声明。例子:

```
python example_test.py IPv4
```
请注意，该脚本也用于自动化测试，因此需要导入IDF测试框架包;

请将'`$IDF_PATH/tools/ci/python_packages `添加到`PYTHONPATH `。


## 硬件要求

本例可以在任何常用的ESP32开发板上运行。

## 配置项目

```
idf.py menuconfig
```

在“Example Configuration Options”中设置以下参数:

* Set `IP version` 例如IPV4或IPV6。

* Set `IPV4 Address` 如果您选择的IP版本IPV4以上。

* Set `IPV6 Address` 如果您选择的IP版本IPV6以上。

* Set `Port` 表示示例将连接到的远程端口

在“连接配置示例”菜单下配置Wi-Fi或以太网。参见[examples/protocols/README.md](../../README.md)中的“建立Wi-Fi或以太网连接”一节了解更多细节。


## 编译和下载

编译项目并将其flash到板上，然后运行monitor工具查看串行输出:

```
idf.py -p PORT flash monitor
```

(如果需要退出monitor, 输入 ``Ctrl-]``.)

有关配置和使用ESP-IDF构建项目的完整步骤，请参阅入门指南。
