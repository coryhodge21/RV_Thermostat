# CMakeLists.txt for App component
idf_component_register(
    SRCS 
        "app.c"
        "tasks.c" 
    INCLUDE_DIRS 
        "include"
    PRIV_REQUIRES 
        freertos
    REQUIRES 
        driver
    )
