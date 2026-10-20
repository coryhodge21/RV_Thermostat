/**
 * @file: i2c_interface.h
 * @date: 3/22/2026
 * @author: Cory W. Hodge
 * @brief : Interface to the i2c driver for setting up 
 *              device as master or slave
 * @note : Developed for i2c driver version 5.x+
 * 
 * @documentation: https://docs.espressif.com/projects/esp-idf/en/stable/esp32/api-reference/peripherals/i2c.html
 * 
 * @developer: 
 *  By default, the I2C interrupt will be deferred when the cache is disabled for reasons like writing or erasing flash. Thus the event callback functions will not get executed in time, which is not expected in a real-time application.
 *
 * There's a Kconfig option CONFIG_I2C_ISR_IRAM_SAFE that will:
 *
 * 1.) Enable the interrupt being serviced even when cache is disabled.
 *
 * 2.) Place all functions that used by the ISR into IRAM.
 *
 * 3.) Place driver object into DRAM (in case it's mapped to PSRAM by accident).
 * 
 * This will allow the interrupt to run while the cache is disabled but will come at the cost of increased IRAM consumption.
 */
#ifndef _I2C_INTERFACE_
#define _I2C_INTERFACE_

#include "driver/i2c_types.h"   // provides public types and def's used by newer i2c drivers

// for vTaskDelay
#include "freertos/FreeRTOS.h"

#define I2C_PORT I2C_NUM_0
#define I2C_MASTER_ADDR 0x00
#define I2C_SLAVE_1_ADDR 0x01
#define I2C_SLAVE_2_ADDR 0x02
#define I2C_SLAVE_3_ADDR 0x03

#define I2C_BUS_TX_BUF_LEN 100
#define I2C_BUS_RX_BUF_LEN 100

extern TaskHandle_t xI2CMasterTaskHandle;
extern TaskHandle_t xI2CSlaveTaskHandle;
 
/**
 * i2c_master_task
 * @breif: task for running the device as i2c master
 */
void i2c_master_task(void * pvParamters);

/**
 * i2cConfigMaster
 * @breif: encapsulate setup routine for i2c bus as master device.
 *          The Master Device is responsible for configuring
 *              and initializing both the: 
 *          - I2C Bus (master bus)
 *          - I2C Master Device (device registers to bus)
 */
void i2cConfigMasterBus(i2c_master_bus_handle_t bus_handle);

/**
 * i2cAssDeviceToMasterBus
 * @brief: Add a device to the Bus while esp32 in Master Mode
 */
void i2cAddDeviceToMasterBus(i2c_master_bus_handle_t bus_handle, i2c_master_dev_handle_t dev_handle);

/**
 * i2c_slave_task
 * @breif: task for running the device as i2c slave
 */
void i2c_slave_task(void * pvParamters);

/**
 * i2cConfigSlave
 * @breif: encapsulate setup routine for i2c as slave devic
 */
//void i2cConfigSlave(i2c_slave_dev_handle_t slave_dev_handle, uint8_t slave_addr);


#endif // _I2C_INTERFACE_