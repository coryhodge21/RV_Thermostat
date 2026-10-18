idf_component_register(
    SRCS "uart_driver.c" 
    INCLUDE_DIRS "."
    REQUIRES 
        freertos
        # for driver/uart.h
        esp_driver_uart
        esp_driver_gpio     # for gpio_t
    PRIV_REQUIRES 
        freeRTOS_CLI # freeRTOS CLI extension
    )