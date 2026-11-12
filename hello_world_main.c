#include <stdio.h>
#include <stdlib.h>
#include <errno.h>

#include "esp_console.h"
#include "esp_err.h"
#include "esp_check.h"

#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "driver/ledc.h"
#include "driver/i2c_master.h"

#define I2C_SDA GPIO_NUM_21     //SDA 
#define I2C_SCL GPIO_NUM_22     //SCL
#define I2C_MASTER_PORT I2C_NUM_0   //port

#define SERVO_PIN_FIRST GPIO_NUM_18     
#define SERVO_CHANNEL LEDC_CHANNEL_0     
#define TIMER LEDC_TIMER_0      
#define SERVO_MODE LEDC_LOW_SPEED_MODE    



//BUS and MASTER config
void master_bus_conf(){
    i2c_master_bus_config_t master_config = {
        .clk_source = I2C_CLK_SRC_DEFAULT,
        .i2c_port = I2C_MASTER_PORT,
        .scl_io_num = I2C_SCL,
        .sda_io_num = I2C_SDA,
        .glitch_ignore_cnt = 7,
        .flags.enable_internal_pullup = true,
    }; 

    static i2c_master_bus_handle_t bus_handle;     //BUS handler
    ESP_ERROR_CHECK(esp_errori2c_new_master_bus(&master_config, &bus_handle));      //new BUS



//comunication MASTER -> SLAVE
//device(slave) config
    i2c_device_config_t device = {
        .dev_addr_length = I2C_ADDR_BIT_LEN_7,
        .device_address = 0x28,
        .scl_speed_hz = 100000,
    };

    i2c_master_dev_handle_t device_handle;
    ESP_ERROR_CHECK(i2c_master_bus_add_device(bus_handle, &device, &device_handle));
}







//servo1 config (timer and channel)
void servo_init(){
    ledc_timer_config_t timer = {       //TIMER
        .speed_mode = SERVO_MODE,
        .timer_num = TIMER,
        .duty_resolution = LEDC_TIMER_16_BIT,
        .freq_hz = 50,
        .clk_cfg = LEDC_AUTO_CLK,
    };
    ledc_timer_config(&timer);

    ledc_channel_config_t channel = {       //CHANNEL
        .gpio_num = SERVO_PIN_FIRST,
        .speed_mode = SERVO_MODE,
        .channel = SERVO_CHANNEL,
        .timer_sel = TIMER,
        .duty = 0,
        .hpoint = 0,
    };
    ledc_channel_config(&channel);
}

