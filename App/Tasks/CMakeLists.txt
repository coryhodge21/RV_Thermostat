# CMakeLists.txt for App/Tasks component
idf_component_register(
    SRCS 
        "lcd_task.c"       # LCD
    INCLUDE_DIRS 
        "include"
    PRIV_REQUIRES 
        freertos
        LCD_I2C # LCD Interface
        I2C     # I2C Interface
    REQUIRES 
        driver
    )
