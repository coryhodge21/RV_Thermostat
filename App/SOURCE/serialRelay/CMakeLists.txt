# CMakeLists.txt for serialRelay project code
idf_component_register(
    SRCS "serialRelay.c" 
    INCLUDE_DIRS 
        "."
        "../../Configs/"    # for gpio_pin_assign.h
    REQUIRES 
        freertos
        # for driver/uart.h
        esp_driver_uart
        esp_driver_gpio     # for gpio_t
    PRIV_REQUIRES 
        freeRTOS_CLI # freeRTOS CLI extension
        uart    # my driver for uart functionality wrappers
    )