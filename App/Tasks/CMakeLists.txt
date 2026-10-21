# CMakeLists.txt for App/Tasks component
idf_component_register(
    SRCS 
        "tasks.c"     # wrapper for task organization
        "lcd_task.c"  # LCD
        "cli_task.c"  # CLI
    INCLUDE_DIRS 
        "include"
        "../Configs"   # for GPIO pin assignments
        "../SOURCE/lcdController"   # for lcdController module
    PRIV_REQUIRES 
        freertos
        LCD_I2C # LCD Interface
        I2C     # I2C Interface
        freeRTOS_CLI # freeRTOS CLI extension
        uart # my driver for uart functionality wrappers
        CLI # CLI component
        lcdController # lcdController 
    REQUIRES 
        driver
    )
