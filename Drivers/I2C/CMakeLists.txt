# CMakeLists.txt for the I2C driver
idf_component_register(
    SRCS 
        "i2c_bus.c" 
    INCLUDE_DIRS 
        "."
    PRIV_REQUIRES 
        freertos
    REQUIRES 
        driver
        esp_driver_i2c
    )
