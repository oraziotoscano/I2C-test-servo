#include <stdio.h>
#include <stdlib.h>
#include <errno.h>

#include "esp_console.h"
#include "esp_err.h"
#include "esp_check.h"

#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "driver/ledc.h"
#include "driver/gpio.h"
#include "driver/i2c_slave.h"

#define I2C_SDA GPIO_NUM_21     //SDA 
#define I2C_SCL GPIO_NUM_22     //SCL
#define I2C_SLAVE_PORT  I2C_NUM_1

#define SERVO_PIN_SECOND GPIO_NUM_19
#define SERVO_CHANNEL LEDC_CHANNEL_0
#define TIMER LEDC_TIMER_0
#define SERVO_MODE LEDC_LOW_SPEED_MODE

void slave_conf(){
    i2c_slave_config_t slave_config = {
        .i2c_port = I2C_SLAVE_PORT,
        .clk_source = I2C_CLK_SRC_DEFAULT,
        .scl_io_num = I2C_SCL,
        .sda_io_num = I2C_SDA,
        .slave_addr = 0x28,     
        .send_buf_depth = 100,
        .receive_buf_depth = 100,
    };

    i2c_slave_dev_handle_t slave_handle;    //slave handle
    ESP_ERROR_CHECK(i2c_new_slave_device(&slave_config, &slave_handle));    //new slave
}
