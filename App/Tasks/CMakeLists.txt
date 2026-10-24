# CMakeLists.txt for App/Tasks component
idf_component_register(
    SRCS 
        "."
        "tasks.c"     # wrapper for task organization
        "lcd_task.c"  # LCD
        "cli_task.c"  # CLI
        "gpio_task.c" # gpio
        "uart1_task.c" # uart1
    INCLUDE_DIRS 
        "include"
        "../Configs"   # for GPIO pin assignments
        "../SOURCE/lcdController"   # for lcdController module
       # "../Drivers/gpio"
        "../../Drivers/uart"   #uart1_task uses uart_driver wrapper fx's (RV_Thermostat/Drivers/uart)
    PRIV_REQUIRES 
        freertos
        LCD_I2C # LCD Interface
        I2C     # I2C Interface
        freeRTOS_CLI # freeRTOS CLI extension
        uart # my driver for uart functionality wrappers
        CLI # CLI component
        lcdController # lcdController
        gpio    # gpio Driver
        uart    # uart wrapper
        DEV     # dev interface
    REQUIRES 
        driver
    )
