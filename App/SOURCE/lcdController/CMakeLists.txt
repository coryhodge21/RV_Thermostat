# CMakeLists.txt for the LCD CONTROLLER
idf_component_register(
    SRCS "lcdController.c" 
    INCLUDE_DIRS "."
    REQUIRES 
        freertos
    #PRIV_REQUIRES 
    #    freeRTOS_CLI # freeRTOS CLI extension
    )