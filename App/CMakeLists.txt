# CMakeLists.txt for App component
idf_component_register(
    SRCS 
        "app.c"         # Application entry point
        "tasks.c"       # wrapper for task organization
        # tasks
    INCLUDE_DIRS 
        "include"
        "Configs"
    PRIV_REQUIRES 
        freertos
        Tasks
    REQUIRES 
        driver
    )
