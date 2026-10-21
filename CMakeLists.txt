# The following five lines of boilerplate have to be in your project's
# CMakeLists in this exact order for cmake to work correctly
cmake_minimum_required(VERSION 3.16)

message(STATUS "Configuring the project... ${CMAKE_CURRENT_LIST_DIR}")

# Components :
set(EXTRA_COMPONENT_DIRS 

    ## RESOURCES ##

    # Resources directory
    "${CMAKE_CURRENT_LIST_DIR}/Resources"
    # freeRTOS_CLI extension
    "${CMAKE_CURRENT_LIST_DIR}/Resources/freeRTOS_CLI"
    
    # git_hash
    "${CMAKE_CURRENT_LIST_DIR}/git_hash"
    
    ## DRIVERS ##

    # Drivers : GPIO
    "${CMAKE_CURRENT_LIST_DIR}/Drivers/gpio"
    # Drivers : I2C
    "${CMAKE_CURRENT_LIST_DIR}/Drivers/I2C"
    # Drivers : LCD_I2C
    "${CMAKE_CURRENT_LIST_DIR}/Drivers/LCD_I2C"
    # Drivers : UART
    "${CMAKE_CURRENT_LIST_DIR}/Drivers/uart"
    # Drivers : WIFI
    "${CMAKE_CURRENT_LIST_DIR}/Drivers/wifi"

    ## APPLICATION ##

    # Main Application
    "${CMAKE_CURRENT_LIST_DIR}/App"
    # App DEV Tools
    "${CMAKE_CURRENT_LIST_DIR}/App/DEV"
    # App CLI
    "${CMAKE_CURRENT_LIST_DIR}/App/CLI"
    # App Configurations
    "${CMAKE_CURRENT_LIST_DIR}/App/Configs"

    # Tasks 
    "${CMAKE_CURRENT_LIST_DIR}/App/Tasks"

    # Source Code
    "${CMAKE_CURRENT_LIST_DIR}/App/SOURCE"
    # App Source Code : UART
    "${CMAKE_CURRENT_LIST_DIR}/App/SOURCE/serialRelay"
    # App Source Code : LCD Controller
    "${CMAKE_CURRENT_LIST_DIR}/App/SOURCE/lcdController"
)

# Output Build informationaaa
# message(STATUS "EXTRA_COMPONENT_DIRS: ${EXTRA_COMPONENT_DIRS}")
# message(STATUS "CMAKE_CURRENT_LIST_DIR: ${CMAKE_CURRENT_LIST_DIR}")

#add_library(serialRelay "${CMAKE_CURRENT_LIST_DIR}/App/SOURCE/uart/serialRelay.c")
#message(STATUS "CMAKE_CURRENT_LIST_DIR: ${CMAKE_CURRENT_LIST_DIR}")


# Include CMAKE from esp-idf
include($ENV{IDF_PATH}/tools/cmake/project.cmake)

# Set the project name
project(RV_Thermostat)

# Add git hash functionality
include(./git_hash/CheckGit.cmake)
CheckGitSetup()

# ## Build Vars for components to reference ##
# set(_PROJECT_DIR_ "${CMAKE_CURRENT_LIST_DIR}/" CACHE STRING "./" )
# set(_APP_DIR_ "${CMAKE_CURRENT_LIST_DIR}/App/" CACHE STRING "./App/")
# set(_APP_CONFIG_DIR_ "${CMAKE_CURRENT_LIST_DIR}/App/Configs/" CACHE STRING "./App/Configs/")
# set(_APP_SOURCE_DIR_ "${CMAKE_CURRENT_LIST_DIR}/App/SOURCE/" CACHE STRING "./App/SOURCE/" )
# set(_DRIVERS_DIR_ "${CMAKE_CURRENT_LIST_DIR}/Drivers/" CACHE STRING "./Drivers/" )
# set(_RESOURCES_DIR_ "${CMAKE_CURRENT_LIST_DIR}/Resources/" CACHE STRING "./Resources/" )
