# CMakeLists.txt for the CLI component
idf_component_register(
    SRCS "cli.c" 
    INCLUDE_DIRS "."
    REQUIRES 
        freertos
    PRIV_REQUIRES 
        freeRTOS_CLI # freeRTOS CLI extension
    
    )
