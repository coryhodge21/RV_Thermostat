# CMakeLists.txt for the freeRTOS_CLI component
idf_component_register(
    SRCS "FreeRTOS_CLI.c" 
    INCLUDE_DIRS "."
    REQUIRES freertos
    )
