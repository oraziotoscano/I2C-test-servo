idf_component_register(
    SRCS "hello_world_main.c"
         "slave.c"
    INCLUDE_DIRS "."
    REQUIRES
        console
        esp_driver_ledc
        esp_driver_i2c
        esp_driver_gpio
)