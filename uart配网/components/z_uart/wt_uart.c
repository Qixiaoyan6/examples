#include "esp_log.h"
#include "wt_uart.h"
static const char *TAG = "WT_UART";

void wt_uart_init()
{
    /* Configure parameters of an UART driver,
     * communication pins and install the driver */
    uart_config_t uart_config = {
        .baud_rate = WT_UART_BAUD_RATE,     //波特率大小
        .data_bits = UART_DATA_8_BITS,      //数据位  
        .parity = UART_PARITY_DISABLE,      //禁用奇偶效验位
        .stop_bits = UART_STOP_BITS_1,      //停止位
        .flow_ctrl = UART_HW_FLOWCTRL_DISABLE,  //禁用硬件流控模式
        .source_clk = UART_SCLK_DEFAULT,      //设置时钟源为SCLK_DEFAULT       
    };
    int intr_alloc_flags = 0;

#if CONFIG_UART_ISR_IN_IRAM
    intr_alloc_flags = ESP_INTR_FLAG_IRAM;
#endif

    ESP_ERROR_CHECK(uart_driver_install(WT_UART_PORT_NUM, WT_UART_BUF_SIZE * 2, 0, 0, NULL, intr_alloc_flags));     //端口号、tx和rx的环形缓冲区大小
    ESP_ERROR_CHECK(uart_param_config(WT_UART_PORT_NUM, &uart_config));                                             //调用已经配置好数据的函数uart_config
    ESP_ERROR_CHECK(uart_set_pin(WT_UART_PORT_NUM, WT_UART_TXD, WT_UART_RXD, WT_UART_RTS, WT_UART_CTS));            //设置引脚

    ESP_LOGI(TAG, "uart init final");
}
