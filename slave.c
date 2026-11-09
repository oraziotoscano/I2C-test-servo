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

#define I2C_SDA GPIO_NUM_21     //SDA 
#define I2C_SCL GPIO_NUM_22     //SCL

#define SERVO_PIN_SECOND GPIO_NUM_19
#define SERVO_CHANNEL LEDC_CHANNEL_0
#define TIMER LEDC_TIMER_0
#define SERVO_MODE LEDC_LOW_SPEED_MODE