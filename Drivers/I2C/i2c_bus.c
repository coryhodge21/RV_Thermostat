/**
 * @file i2c_bus.c
 * @date 4/21/2026
 * @author Cory W. Hodge
 * @breif calling module to control LCD with lcd exposed api's
 * This file contains the implementation of the I2C bus initialization 
 * and device addition functions, as well as the I2C master task that 
 * handles communication with I2C devices. The I2C master task will transmit 
 * and receive data from connected I2C devices based on a switch case mechanism.
 * The I2C bus is configured with specific pin assignments and settings, and devices can be
 * added to the bus with defined configurations. The task will run indefinitely, allowing for 
 * continuous communication with I2C devices.
 */

// for common point i2c access definitions
//#include "i2c_interface.h"

// I2C Bus Interface
 #include "i2c_bus.h"

 // for ESP_ERROR_CHECK() and esp_rom_delay_us()
#include <esp_err.h>

// For LCD
//#include "lcd_i2c.h"

// File TAG
//static const char * TAG = "i2c_master";

 /**
 * Global Task Handle
 */
//TaskHandle_t xI2CBusTaskHandle = NULL;

/**
 * initialize_i2c_bus
 * @param bus_handle The handle for the I2C bus (output parameter)
 * This function initializes the I2C bus with the specified configurations,
 * including pin assignments and clock settings. It creates a new master 
 * bus and assigns it to the provided handle.
 */
void initialize_i2c_bus(i2c_master_bus_handle_t * bus_handle, i2c_clock_source_t clk_source, i2c_port_t i2c_port, gpio_num_t sda_io_num, gpio_num_t scl_io_num, bool enable_pullup)
{
   // Master Bus Configurations
   i2c_master_bus_config_t i2c_mst_config = 
   {
       .clk_source = clk_source,
       .i2c_port = i2c_port,
       .scl_io_num = scl_io_num,
       .sda_io_num = sda_io_num,
       .glitch_ignore_cnt = 7,
       .flags.enable_internal_pullup = enable_pullup,
   };

   // Install Master Bus
   ESP_ERROR_CHECK(i2c_new_master_bus(&i2c_mst_config, bus_handle));
}

/**
 * i2cAddSlaveToBus
 * @param bus_handle The handle for the I2C bus
 * @param dev_handle The handle for the I2C device (output parameter)
 * @param device_address The 7-bit address of the I2C device
 * @param scl_speed_hz The clock speed for the I2C communication in Hz
 * This function adds a slave device to the I2C master bus with the specified configurations.
 */
void i2cAddSlaveToBus(i2c_master_bus_handle_t bus_handle, i2c_master_dev_handle_t * dev_handle, uint32_t scl_speed_hz, uint16_t dev_addr)
{
    // Slave Device Configurations
    i2c_device_config_t dev_cfg = 
    {
        .dev_addr_length = I2C_ADDR_BIT_LEN_7,
        .device_address = dev_addr,
        .scl_speed_hz = scl_speed_hz,
    };
    
    // Install Master Device on Master Bus
    ESP_ERROR_CHECK(i2c_master_bus_add_device(bus_handle, &dev_cfg, dev_handle));
}