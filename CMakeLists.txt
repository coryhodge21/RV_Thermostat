# The following five lines of boilerplate have to be in your project's
# CMakeLists in this exact order for cmake to work correctly
cmake_minimum_required(VERSION 3.16)

message(STATUS "Configuring the project... ${CMAKE_CURRENT_LIST_DIR}")

# Components :
set(EXTRA_COMPONENT_DIRS 
    # Resources directory
    "${CMAKE_CURRENT_LIST_DIR}/Resources"
    # freeRTOS_CLI extension
    "${CMAKE_CURRENT_LIST_DIR}/Resources/freeRTOS_CLI"
    # Main Application
    "${CMAKE_CURRENT_LIST_DIR}/App"
    # Application CLI
    "${CMAKE_CURRENT_LIST_DIR}/App/CLI"
    # Configurations
    "${CMAKE_CURRENT_LIST_DIR}/App/Configs"
    # DEV
    "${CMAKE_CURRENT_LIST_DIR}/App/DEV"
    # Tasks 
    "${CMAKE_CURRENT_LIST_DIR}/App/Tasks"
    # Drivers : GPIO
    "${CMAKE_CURRENT_LIST_DIR}/Drivers/gpio"
    # Drivers : I2C
    "${CMAKE_CURRENT_LIST_DIR}/Drivers/I2C"
    # Drivers : LCD_I2C
    "${CMAKE_CURRENT_LIST_DIR}/Drivers/LCD_I2C"
    # Drivers : UART
    "${CMAKE_CURRENT_LIST_DIR}/Drivers/uart"
    
)

# Output Build informationaaa
message(STATUS "EXTRA_COMPONENT_DIRS: ${EXTRA_COMPONENT_DIRS}")

# Include CMAKE from esp-idf
include($ENV{IDF_PATH}/tools/cmake/project.cmake)

# Set the project name
project(RV_Thermostat)

# Add git hash functionality
include(./git_hash/CheckGit.cmake)
CheckGitSetup()