/**
 * @file: i2c_interface_master.c
*/
// for i2c Macro
#include "i2c_interface.h"
// For pin assignments
#include "gpio_pin_assign.h"    
// for i2c driver
#include "driver/i2c_master.h"  
// for ESP_LOGx()
#include "esp_log.h"    

// File TAG
static const char * TAG = "i2c_master";

/**
 * TASK HANDLE
 */
TaskHandle_t xI2CMasterTaskHandle = NULL;

// i2cConfigureMaster
void i2cConfigMasterBus(i2c_master_bus_handle_t bus_handle)
{
    
    /*  Master Bus  */

    // Master Bus Configurations
    i2c_master_bus_config_t i2c_mst_config = 
    {
        .clk_source = I2C_CLK_SRC_DEFAULT,
        .i2c_port = I2C_PORT,
        .scl_io_num = I2C_MASTER_SCL,
        .sda_io_num = I2C_MASTER_SDA,
        .glitch_ignore_cnt = 7,
        .flags.enable_internal_pullup = true,
    };

    // Install Master Bus
    ESP_ERROR_CHECK(i2c_new_master_bus(&i2c_mst_config, &bus_handle));

}

// Add device to master bus
void i2cAddDeviceToMasterBus(i2c_master_bus_handle_t bus_handle, i2c_master_dev_handle_t dev_handle)
{

    /*  Master Device   */

    // Master Device Configurations
    i2c_device_config_t dev_cfg = 
    {
        .dev_addr_length = I2C_ADDR_BIT_LEN_7,
        .device_address = 0x00,
        .scl_speed_hz = 100000,
    };
    
    // Install Master Device on Master Bus
    ESP_ERROR_CHECK(i2c_master_bus_add_device(bus_handle, &dev_cfg, &dev_handle));

}

/**
 * I2C Master Task
 * 
 * 
 */
void i2c_master_task(void * pvParamters)
{
    // Master Bus Handle
    i2c_master_bus_handle_t bus_handle = NULL;
    
    // Master Device Handle
    i2c_master_dev_handle_t dev_handle = NULL;

    // i2cConfigureMaster(bus_handle, dev_handle);

    // // using SLAVE_AADR_1 for testing
    // i2cAddDeviceToMasterBus(bus_handle, dev_handle)

        /*  Master Bus  */

    // Master Bus Configurations
    i2c_master_bus_config_t i2c_mst_config = 
    {
        .clk_source = I2C_CLK_SRC_DEFAULT,
        .i2c_port = I2C_PORT,
        .scl_io_num = I2C_MASTER_SCL,
        .sda_io_num = I2C_MASTER_SDA,
        .glitch_ignore_cnt = 7,
        .flags.enable_internal_pullup = true,
    };
 
    // Install Master Bus
    ESP_ERROR_CHECK(i2c_new_master_bus(&i2c_mst_config, &bus_handle));

    /*  Master Device   */

    // Master Device Configurations
    i2c_device_config_t dev_cfg = 
    {
        .dev_addr_length = I2C_ADDR_BIT_LEN_7,
        .device_address = I2C_SLAVE_1_ADDR,
        .scl_speed_hz = 100000,
    };
    
    // Install Master Device on Master Bus
    ESP_ERROR_CHECK(i2c_master_bus_add_device(bus_handle, &dev_cfg, &dev_handle));

    ///////////////////////////////////////////////////////

    if(dev_handle == NULL || bus_handle == NULL)
    {
        ESP_LOGI(TAG, "Could not initialize");
        abort();
    }

    // Test Payload
    uint8_t payload[20] = {"RIO"};
    uint8_t recBuffer[100] = {0};
    esp_err_t esp_ret = ESP_OK;
    int SWITCH = 1;

    // Task Loop
    while(1)
    {
        vTaskDelay(1000 / portTICK_PERIOD_MS);

        switch(SWITCH)
        {
            case 1:
                //ESP_LOGI(TAG, "master_tx()");
                esp_ret = i2c_master_transmit_receive(dev_handle, payload, sizeof(payload), recBuffer, 256, -1);
            break;

            case 2:
                esp_ret = i2c_master_transmit(dev_handle, payload, sizeof(payload), -1);

            break;
                esp_ret = i2c_master_receive(dev_handle, recBuffer, 256, -1);
            case 3:

            break;

            default:

            break;
        }

        // Increment or cycle
        (SWITCH == 3) ? SWITCH = 1 : SWITCH++;

        if(esp_ret != ESP_OK)
        {
            ESP_LOGI("MASTER", "couould not read responpse\r\n");
            continue;
        }


        ESP_LOGI("MASTER", "RECV:");
        for(int i = 0; i < 7; i++)
        {
            //sprintf(asciiBuffer, "%c", recBuffer[i]);
            //ESP_LOGI("MASTER","%u", recBuffer[i]);
            ESP_LOGI("MASTER","%c", recBuffer[i]);
        }
    }
}

/**
 * Read / Write Commands :
 * 
 * i2c_master_transmit(dev_handle, data_wr, DATA_LENGTH, -1);
 * 
 * i2c_master_receive(dev_handle, data_rd, DATA_LENGTH, -1);
 * 
 * i2c_master_transmit_receive(dev_handle, buf, sizeof(buf), buffer, 2, -1);
 * 
 * i2c_master_transmit_multi_buffer_info_t lcd_i2c_buffer[3] = {
 *   {.write_buffer = &control_phase_byte, .buffer_size = control_phase_size},
 *   {.write_buffer = cmd_buffer, .buffer_size = cmd_buffer_size},
 *   {.write_buffer = lcd_buffer, .buffer_size = lcd_buffer_size},
 * };
 *
 * i2c_master_multi_buffer_transmit(handle, lcd_i2c_buffer, sizeof(lcd_i2c_buffer) / sizeof(i2c_master_transmit_multi_buffer_info_t), -1);
 */