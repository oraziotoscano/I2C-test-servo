#include <stdio.h>
#include<stdlib.h>
#include <errno.h>

#include "esp_console.h"
#include "esp_err.h"
#include "esp_check.h"

#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
//#include "driver/uart.h"
#include "driver/ledc.h"

#define SERVO_PIN_FIRST      18
#define SERVO_PIN_SECOND     21
#define SERVO_CHANNEL_FIRST  LEDC_CHANNEL_0
#define SERVO_CHANNEL_SECOND LEDC_CHANNEL_1
#define SERVO_TIMER    LEDC_TIMER_0
#define SERVO_MODE     LEDC_LOW_SPEED_MODE

//------------------------------------------------------
// Inizializza il PWM
//------------------------------------------------------

void servo2_init(void)
{
    ledc_timer_config_t timer = {
        .speed_mode = SERVO_MODE,
        .timer_num = SERVO_TIMER,
        .duty_resolution = LEDC_TIMER_16_BIT,
        .freq_hz = 50,
        .clk_cfg = LEDC_AUTO_CLK
    };

    ledc_timer_config(&timer);

    ledc_channel_config_t channel = {
        .gpio_num = SERVO_PIN_SECOND,
        .speed_mode = SERVO_MODE,
        .channel = SERVO_CHANNEL_SECOND,
        .timer_sel = SERVO_TIMER,
        .duty = 0,
        .hpoint = 0
    };

    ledc_channel_config(&channel);
}




void servo1_init(void)
{
    ledc_timer_config_t timer = {
        .speed_mode = SERVO_MODE,
        .timer_num = SERVO_TIMER,
        .duty_resolution = LEDC_TIMER_16_BIT,
        .freq_hz = 50,
        .clk_cfg = LEDC_AUTO_CLK
    };

    ledc_timer_config(&timer);

    ledc_channel_config_t channel = {
        .gpio_num = SERVO_PIN_FIRST,
        .speed_mode = SERVO_MODE,
        .channel = SERVO_CHANNEL_FIRST,
        .timer_sel = SERVO_TIMER,
        .duty = 0,
        .hpoint = 0
    };

    ledc_channel_config(&channel);
}



void servo_write(int angle)
{
    if (angle < 0) angle = 0;
    if (angle > 180) angle = 180;

    // 0° -> 1638
    // 180° -> 4915

    int duty = 1638 + (angle * 3277) / 180;

    ledc_set_duty(SERVO_MODE, SERVO_CHANNEL_FIRST, duty);
    ledc_update_duty(SERVO_MODE, SERVO_CHANNEL_FIRST);

    ledc_set_duty(SERVO_MODE, SERVO_CHANNEL_SECOND, duty);
    ledc_update_duty(SERVO_MODE, SERVO_CHANNEL_SECOND);
}


static int cmd_servo(int argc, char **argv)
{
    if (argc != 2) {
        printf("Uso: servo <angolo 0-180>\n");
        return 1;
    }

    char *end;
    errno = 0;
    long angle = strtol(argv[1], &end, 10);

    if (errno != 0 || end == argv[1] || *end != '\0' || angle < 0 || angle > 180) {
        printf("Angolo non valido. Inserisci un intero tra 0 e 180.\n");
        return 1;
    }

    servo_write((int)angle);
    printf("Servo impostato a %ld gradi.\n", angle);

    return 0;
}



static void console_init(void)
{
    esp_console_repl_t *repl = NULL;
    esp_console_repl_config_t repl_config = ESP_CONSOLE_REPL_CONFIG_DEFAULT();

    repl_config.prompt = "ESP32>";

    // 1. Crea la REPL: inizializza anche la console
    esp_err_t err = esp_console_new_repl_stdio(&repl_config, &repl);

    if (err != ESP_OK) {
        printf("Creazione REPL: %s (0x%x)\n",esp_err_to_name(err), (unsigned int)err);
        return;
    }

    // 2. Registra il comando dopo aver inizializzato la console
    esp_console_cmd_t servo_cmd = {
        .command = "servo",
        .help = "Sposta il servo: servo <angolo 0-180>",
        .hint = NULL,
        .func = &cmd_servo,
    };

    err = esp_console_cmd_register(&servo_cmd);

    if (err != ESP_OK) {
        printf("Registrazione comando: %s\n",esp_err_to_name(err));
        return;
    }

    // 3. Avvia la REPL
    err = esp_console_start_repl(repl);

    if (err != ESP_OK) {
        printf("Avvio REPL: %s\n", esp_err_to_name(err));
    }
}



/*
void servo_start(){
    int duty = 1638 + (0 * 3277) / 180;

    ledc_set_duty(SERVO_MODE, SERVO_CHANNEL, duty);
    ledc_update_duty(SERVO_MODE, SERVO_CHANNEL);
}
*/


//------------------------------------------------------
// Programma principale
//------------------------------------------------------

void app_main(void){
    printf("Inizializzazione servo...\n");

    servo1_init();
    servo2_init();
    servo_write(0);

    console_init();


    
    
}