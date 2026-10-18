# CMakeLists.txt for the CLI component
idf_component_register(
    SRCS 
        "cli.c" 
    INCLUDE_DIRS 
        "."
        "../Configs"   # for GPIO pin assignments
    REQUIRES 
        freertos
        esp_driver_uart # for driver/uart.h
    PRIV_REQUIRES 
        freeRTOS_CLI # freeRTOS CLI extension
        uart # my driver for uart functionality wrappers

    )
