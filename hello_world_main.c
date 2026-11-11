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
}

