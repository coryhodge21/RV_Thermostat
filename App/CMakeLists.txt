# CMakeLists.txt for App component
idf_component_register(
    SRCS 
        "app.c"         # Application entry point
    INCLUDE_DIRS 
        "include"
        "Configs"
        "DEV"
        "CLI"
        "Tasks"
    PRIV_REQUIRES 
        freertos
        Tasks
    REQUIRES 
        driver
    )
