# CMakeLists.txt for DEV component
idf_component_register(
    SRCS 
        "dev.c"        
    INCLUDE_DIRS 
        "."
        "../Configs"    # for GPIO pin assignments
    PRIV_REQUIRES 
        freertos
        Tasks
    REQUIRES 
        driver
        uart    # my driver for uart functionality wrappers
    )
