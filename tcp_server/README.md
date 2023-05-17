| Supported Targets | ESP32 | ESP32-C2 | ESP32-C3 | ESP32-S2 | ESP32-S3 |
| ----------------- | ----- | -------- | -------- | -------- | -------- |


# TCP Server 示例

应用程序创建具有指定端口号的TCP服务端，并等待来自客户端的连接请求。在接受来自客户端的请求后，服务器和客户端之间建立连接，应用程序等待从客户端接收一些数据。接收到的数据被打印为ASCII文本并重传回客户端。

## 如何使用示例

要创建与TCP服务器通信的TCP客户端示例，请选择以下选项之一。

有许多主机端工具可用于与UDP/TCP服务器/客户端进行交互。

一个命令行工具是[netcat](http://netcat.sourceforge.net)，它可以发送和接收多种数据包。

注意:请将“192.168.0.167 3333”替换为所需的IPV4/IPV6地址(显示在监控控制台)和端口号。

除了这些工具之外，还可以在sockets/scripts目录下找到简单的Python脚本。每个脚本都设计为与其中一个示例交互。

### Python脚本
脚本example_test.py可以用作tcp-服务器应用程序的对应部分，

IP地址和要发送到服务器的消息应作为参数声明。示例:

```
python example_test.py 192.168.0.167 Message
```
请注意，该脚本也用于自动化测试，因此需要导入IDF测试框架包;

请将`$IDF_PATH/tools/ci/python_packages `添加到` PYTHONPATH `。

## 硬件要求

本例可以在任何常用的ESP32开发板上运行。

## Configure the project

```
idf.py menuconfig
```

在“Example Configuration Options”中设置以下参数:

* Set `IP version` 取值为IPV4或IPV6。

* Set `Port` 服务器示例将创建的端口号。

* Set `TCP keep-alive idle time(s)`TCP保持空闲时间的值。这个时间是最后一次数据传输之间的时间。

* Set `TCP keep-alive interval time(s)`设置TCP保持连接的时间间隔。该时间为keepalive探测报文的间隔时间。

* Set `TCP keep-alive packet retry send counts` 设置TCP keep alive报文重试发送次数。这是keepalive探测报文的重试次数。

在“连接配置示例”菜单下配置Wi-Fi或以太网。参见[examples/protocols/README.md](../../README.md)中的“建立Wi-Fi或以太网连接”一节了解更多细节。

## 编译和下载

构建项目并将其flash到板上，然后运行monitor工具查看串行输出:

```
idf.py -p PORT flash monitor
```

(如果需要退出monitor, 输入 ``Ctrl-]``.)

有关配置和使用ESP-IDF构建项目的完整步骤，请参阅入门指南。
