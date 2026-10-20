/**
 * @file: i2c_interface_slave.c
 */
// loging
#include "esp_log.h"
// for i2c Macro
#include "i2c_interface.h"
// For pin assignments
#include "gpio_pin_assign.h"    
// for i2c driver
#include "driver/i2c_slave.h"  

#include "freeRTOS/queue.h"

// https://docs.espressif.com/projects/esp-idf/en/stable/esp32/api-reference/peripherals/i2c.html#i2c_slave_controller


//static const char *TAG = "I2C_SLAVE";

/**
 * TASK HANDLE
 */
TaskHandle_t xI2CSlaveTaskHandle = NULL;
bool ISR_RX_DONE_INT = false;
bool ISR_REQ_INT = false;
uint8_t ISR_RECV_BUFFER[100] = {0};

// i2cConfigSlave
// Attention : using address I2C_SLAVE_1_ADDR
esp_err_t init_i2c_slave(i2c_slave_dev_handle_t * slave_handle, uint8_t slave_addr)
{

    // Configurations for Slave Device
    i2c_slave_config_t i2c_slv_config = 
    {
        .i2c_port = I2C_PORT,
        .clk_source = I2C_CLK_SRC_DEFAULT,    // Use default clock
        .scl_io_num = I2C_SLAVE_SCL,
        .sda_io_num = I2C_SLAVE_SDA,
        .slave_addr = slave_addr,
        .addr_bit_len = I2C_ADDR_BIT_LEN_7,   // 7-bit address
        .send_buf_depth = I2C_BUS_TX_BUF_LEN,                // TX buffer size
        .receive_buf_depth = I2C_BUS_RX_BUF_LEN,
    };

    // Allocate and initialize the slave device
    esp_err_t esp_ret = i2c_new_slave_device(&i2c_slv_config, slave_handle);

        // Print the pointer address
    ESP_LOGI("I2C_SLAVE", "Slave handle pointer: %p", (void *)slave_handle);

    return esp_ret;
}
/**
 * typedef struct {
    i2c_slave_request_callback_t on_request;        
    i2c_slave_received_callback_t on_receive;        
} i2c_slave_event_callbacks_t;
 */
 
 /*!< Callback for when a master requests data from the slave */
//typedef bool (*i2c_slave_request_callback_t)(i2c_slave_dev_handle_t i2c_slave, const i2c_slave_request_event_data_t *evt_data, void *arg);
bool IRAM_ATTR my_i2c_slave_rx_done_callback(i2c_slave_dev_handle_t i2c_slave, const i2c_slave_rx_done_event_data_t *evt_data, void *arg)
{
    ISR_RX_DONE_INT = true;
    
    return true;
}

 /*!< Callback for when the slave receives data from the master */
//typedef bool (*i2c_slave_received_callback_t)(i2c_slave_dev_handle_t i2c_slave, const i2c_slave_rx_done_event_data_t *evt_data, void *arg);
bool IRAM_ATTR my_i2c_slave_request_callback(i2c_slave_dev_handle_t i2c_slave, const i2c_slave_request_event_data_t *evt_data, void *arg)
{
    ISR_REQ_INT = true;
    
    // for(int i = 0; i < evt_data->length; i++)
    // {
    //     ISR_RECV_BUFFER[i] = evt_data->buffer[i];
    // }

    return true;
}

 // I2C Slave Task
void i2c_slave_task(void * pvParamters)
{
    // Create Handle for Slave device
    i2c_slave_dev_handle_t slave_handle;

    // Add slave device at desired address
    ESP_ERROR_CHECK(init_i2c_slave(&slave_handle, I2C_SLAVE_1_ADDR));

    // Register Callbacks
    char * user_data = "callback_data";

    i2c_slave_event_callbacks_t cbs = 
    {
        .on_request = my_i2c_slave_request_callback,
        .on_receive = my_i2c_slave_rx_done_callback,
    };

    ESP_ERROR_CHECK(i2c_slave_register_event_callbacks(slave_handle, &cbs, user_data));

    //uint32_t count = 0;
    uint8_t slaveResp[100];
    slaveResp[0] = 'R'; // 0x52
    slaveResp[1] = 'I'; // 0x49
    slaveResp[2] = 'O'; // 0x4F
    slaveResp[3] = 'C';
    slaveResp[4] = 'O';
    slaveResp[5] = 'R';
    slaveResp[6] = 'Y';

    uint32_t bytesWritten = 0;

    while(1)
    {
        vTaskDelay(500 / portTICK_PERIOD_MS);

        // This is non-blocking; it puts data into the TX buffer to be read by the master
        ESP_ERROR_CHECK(i2c_slave_write(slave_handle, (const uint8_t *)slaveResp, 7, &bytesWritten, -1));

        if ( ISR_RX_DONE_INT )
        {
            ISR_RX_DONE_INT = false;
            ESP_LOGI("SLAVE", " rx done callback");

        }
        if ( ISR_REQ_INT )
        {
            ISR_REQ_INT = false;
            ESP_LOGI("SLAVE", " rx Req callback");
        }
    }
}
