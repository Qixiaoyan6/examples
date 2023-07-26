#include <stdio.h>
#include <stdint.h>
#include <freertos/FreeRTOS.h>
#include <freertos/task.h>
#include "esp_log.h"
#include "driver/i2c.h"
#include "esp_system.h"

static const char *TAG = "LM75A";

#define I2C_MASTER_SCL_IO CONFIG_I2C_MASTER_SCL /*!< 用于I2C主时钟的GPIO号 */
#define I2C_MASTER_SDA_IO CONFIG_I2C_MASTER_SDA /*!< 用于I2C主数据的GPIO号  */
#define I2C_MASTER_NUM 0                        /*!< I2C主I2C端口号，可用的I2C外设接口数量将取决于芯片 */
#define I2C_MASTER_FREQ_HZ 400000               /*!< I2C主时钟频率 */
#define I2C_MASTER_TX_BUF_DISABLE 0             /*!< I2C主机不需要缓冲区 */
#define I2C_MASTER_RX_BUF_DISABLE 0             /*!< I2C主机不需要缓冲区 */
#define I2C_MASTER_TIMEOUT_MS 1000

#define LM75A_SENSOR_ADDR 0x48
#define LM75A_TEMP_REG_ADDR 0x00

static esp_err_t i2c_write(i2c_port_t i2c_num, uint8_t *data, size_t size)
{
    i2c_cmd_handle_t handle = i2c_cmd_link_create();
    i2c_master_start(handle);
    i2c_master_write_byte(handle, (LM75A_SENSOR_ADDR << 1) | I2C_MASTER_WRITE, true);
    i2c_master_write(handle, data, size, true);
    i2c_master_stop(handle);
    esp_err_t ret = i2c_master_cmd_begin(i2c_num, handle, I2C_MASTER_TIMEOUT_MS / portTICK_PERIOD_MS);
    i2c_cmd_link_delete(handle);
    return ret;
}

static esp_err_t i2c_read(i2c_port_t i2c_num, uint8_t *data, size_t size)
{
    if (size == 0)
    {
        return ESP_OK;
    }
    i2c_cmd_handle_t handle = i2c_cmd_link_create();
    i2c_master_start(handle);
    i2c_master_write_byte(handle, (LM75A_SENSOR_ADDR << 1) | I2C_MASTER_READ, true);
    i2c_master_read(handle, data, size, I2C_MASTER_LAST_NACK);
    i2c_master_stop(handle);
    esp_err_t ret = i2c_master_cmd_begin(i2c_num, handle, I2C_MASTER_TIMEOUT_MS / portTICK_PERIOD_MS);
    i2c_cmd_link_delete(handle);
    return ret;
}

static esp_err_t lm75a_temp_read(uint8_t reg_addr, uint8_t *data, size_t len)
{
    // i2c_master_write_read_device包括了地址访问以及数据读取
#if 1
    esp_err_t ret;
    // i2c选择需要读取的从机寄存器
    ret = i2c_write(I2C_MASTER_NUM, &reg_addr, 1);
    // i2c读取从机寄存器数据
    ret = i2c_read(I2C_MASTER_NUM, data, len);
    return ret;
#else
    return i2c_master_write_read_device(I2C_MASTER_NUM, LM75A_SENSOR_ADDR, &reg_addr, 1, data, len, I2C_MASTER_TIMEOUT_MS / portTICK_PERIOD_MS);
#endif
}

// static esp_err_t lm75a_register_write_byte(uint8_t reg_addr, uint8_t data)
// {
//     int ret;
//     uint8_t write_buf[2] = {reg_addr, data};

//     ret = i2c_master_write_to_device(I2C_MASTER_NUM, LM75A_SENSOR_ADDR, write_buf, sizeof(write_buf), I2C_MASTER_TIMEOUT_MS / portTICK_PERIOD_MS);

//     return ret;
// }

static esp_err_t i2c_master_init(void)
{
    int i2c_master_port = I2C_MASTER_NUM;

    // 配置i2c引脚
    i2c_config_t conf = {
        .mode = I2C_MODE_MASTER,
        .sda_io_num = I2C_MASTER_SDA_IO,
        .scl_io_num = I2C_MASTER_SCL_IO,
        .sda_pullup_en = GPIO_PULLUP_ENABLE,
        .scl_pullup_en = GPIO_PULLUP_ENABLE,
        .master.clk_speed = I2C_MASTER_FREQ_HZ,
    };
    i2c_param_config(i2c_master_port, &conf);

    // 安装i2c驱动
    return i2c_driver_install(i2c_master_port, conf.mode, I2C_MASTER_RX_BUF_DISABLE, I2C_MASTER_TX_BUF_DISABLE, 0);
}

void app_main(void)
{
    // 初始化i2c
    ESP_ERROR_CHECK(i2c_master_init());
    ESP_LOGI(TAG, "I2C initialized successfully");

    uint8_t data[2] = {0};
    uint16_t temp = 0;
    float temp_all = 0;
    while (1)
    {
        lm75a_temp_read(LM75A_TEMP_REG_ADDR, data, sizeof(data));

        // 获取温度数据
        temp = ((data[0] << 8) | data[1]) >> 5;

        // 判断温度是否为正
        if (!(data[0] & 0x80))
        {
            // 计算LM75A温度
            temp_all = (float)temp * 0.125;
        }

        ESP_LOGI(TAG, "The temp is: %.3f", temp_all);
        vTaskDelay(5000 / portTICK_PERIOD_MS);
    }

    // 删除i2c驱动
    ESP_ERROR_CHECK(i2c_driver_delete(I2C_MASTER_NUM));
    ESP_LOGI(TAG, "I2C de-initialized successfully");
}
