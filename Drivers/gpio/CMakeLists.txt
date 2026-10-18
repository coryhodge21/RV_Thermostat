idf_component_register(
    SRCS "gpio_driver.c" 
    INCLUDE_DIRS "."
    PRIV_REQUIRES freertos
    REQUIRES driver
    esp_driver_gpio
    )
