# The following five lines of boilerplate have to be in your project's
# CMakeLists in this exact order for cmake to work correctly
cmake_minimum_required(VERSION 3.16)

message(STATUS "Configuring the project... ${CMAKE_CURRENT_LIST_DIR}")

# Components :

## GPIO
set(EXTRA_COMPONENT_DIRS 
    "${CMAKE_CURRENT_LIST_DIR}/App"
    "${CMAKE_CURRENT_LIST_DIR}/Drivers/gpio"
    "${CMAKE_CURRENT_LIST_DIR}/Drivers/I2C"
    "${CMAKE_CURRENT_LIST_DIR}/Drivers/LCD_I2C"
)

# Output Build informationaaa
message(STATUS "EXTRA_COMPONENT_DIRS: ${EXTRA_COMPONENT_DIRS}")

# Include CMAKE from esp-idf
include($ENV{IDF_PATH}/tools/cmake/project.cmake)

# Set the project name
project(RV_Thermostat)
