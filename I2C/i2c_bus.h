/**
 * @file: i2c_bus.h
 * @date: 4/21/2026
 * @author: Cory W. Hodge
 * @breif: calling module to control LCD with lcd exposed api's
 */
#ifndef _I2C_BUS_H_
#define _I2C_BUS_H_

 // for FreeRTOS library
#include "freertos/FreeRTOS.h"

 // for TaskHandle_t
#include "freertos/task.h"

// for CLI_Command_Definition_t
#include "../freertos_ext/FreeRTOS_CLI.h"

// For pin assignments
#include "gpio_pin_assign.h"

// for i2c Macro
#include "i2c_interface.h"

// for i2c driver
#include "driver/i2c_master.h"

// for ESP_LOGx()
#include "esp_log.h"

// use I2C port 0
#define I2C_PORT I2C_NUM_0


/**
 * Global Task Handle
 */
extern TaskHandle_t xI2CBusTaskHandle;

 /**
  * initialize_i2c_bus
  * @param bus_handle The handle for the I2C bus (output parameter)
  * This function initializes the I2C bus with the specified configurations,
  * including pin assignments and clock settings. It creates a new master 
  * bus and assigns it to the provided handle.
  */
 void initialize_i2c_bus(i2c_master_bus_handle_t * bus_handle, i2c_clock_source_t clk_source, i2c_port_t i2c_port, gpio_num_t sda_io_num, gpio_num_t scl_io_num, bool enable_pullup);

 /**
 * i2cAddSlaveToBus
 * @param bus_handle The handle for the I2C bus
 * @param dev_handle The handle for the I2C device (output parameter)
 * @param device_address The 7-bit address of the I2C device
 * @param scl_speed_hz The clock speed for the I2C communication in Hz
 * This function adds a slave device to the I2C master bus with the specified configurations.
 */
void i2cAddSlaveToBus(i2c_master_bus_handle_t bus_handle, i2c_master_dev_handle_t * dev_handle, uint32_t scl_speed_hz, uint16_t dev_addr);

/**
 * I2C Master Task
 * @param pvParameters Parameters for the task (not used)
 */
void i2c_master_task(void * pvParameters);

#endif // _I2C_BUS_H_