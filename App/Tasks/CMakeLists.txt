# CMakeLists.txt for App/Tasks component
idf_component_register(
    SRCS 
        "tasks.c"     # wrapper for task organization
        "lcd_task.c"  # LCD
    INCLUDE_DIRS 
        "include"
        "../Configs"   # for GPIO pin assignments
    PRIV_REQUIRES 
        freertos
        LCD_I2C # LCD Interface
        I2C     # I2C Interface
        freeRTOS_CLI # freeRTOS CLI extension
    REQUIRES 
        driver
    )
