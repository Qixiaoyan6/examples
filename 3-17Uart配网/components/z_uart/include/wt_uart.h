#pragma once

#include "driver/uart.h"
#include "driver/gpio.h"

#define WT_UART_TXD 6
#define WT_UART_RXD 7
#define WT_UART_RTS (UART_PIN_NO_CHANGE)
#define WT_UART_CTS (UART_PIN_NO_CHANGE)

#define WT_UART_PORT_NUM 1
#define WT_UART_BAUD_RATE 115200

#define WT_UART_BUF_SIZE (1024)

void wt_uart_init();