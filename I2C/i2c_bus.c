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
#include "i2c_interface.h"

// I2C Bus Interface
 #include "i2c_bus.h"

 // for ESP_ERROR_CHECK() and esp_rom_delay_us()
#include <esp_err.h>

// For LCD
#include "lcd_i2c.h"

// File TAG
static const char * TAG = "i2c_master";

 /**
 * Global Task Handle
 */
TaskHandle_t xI2CBusTaskHandle = NULL;

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


/**
 * I2C Master Task
 * @param pvParameters Parameters for the task (not used)
 */
void i2c_master_task(void * pvParameters)
{
    /* Prepare I2C Bus */

    // Master Bus Handle
    i2c_master_bus_handle_t bus_handle = NULL;
    
    // LCD Device Handle ( device to be added to the master bus)
    i2c_master_dev_handle_t lcd_handle = NULL;

    // Initialize I2C Bus
	initialize_i2c_bus(/*bus_handle*/&bus_handle, /*clk_source*/I2C_CLK_SRC_DEFAULT, /*i2c_port*/ I2C_PORT, /*sda_io_num*/ I2C_MASTER_SDA, /*scl_io_num*/ I2C_MASTER_SCL, /*enable_pullup*/ true);

    // Add LCD Device to Master Bus
	i2cAddSlaveToBus(bus_handle, &lcd_handle, LCD_SCL_SPEED_HZ, LCD_ADDR);

    // Check if initialization was successful
    if(bus_handle == NULL ||  lcd_handle == NULL)
    {
        ESP_LOGI(TAG, "Could not initialize"); 
        abort();
    }

    /* LCD */

    // Create LCD Object
    LCD_I2C * lcd = LCD_I2C_Create(bus_handle, lcd_handle);

    // Initialization  Sequence
    lcd_init(lcd);

	// Prepare the LCD screen
    lcd_send_cmd(lcd, LCD_CMD_CLEAR_DISPLAY); 
    lcd_send_cmd(lcd, LCD_CMD_RETURN_HOME);  

	/* Task Loop */
    while(1)
    {
        // Test cursor
        lcd_home(lcd);  // Move cursor to home position (0,0)
        lcd_show_cursor(lcd);
        lcd_blink_cursor(lcd);

        // Test Sent Bytes
		lcd_send_data(lcd, 'H');
		vTaskDelay(500 / portTICK_PERIOD_MS);

		lcd_send_data(lcd, 'E');
		vTaskDelay(500 / portTICK_PERIOD_MS);

		lcd_send_data(lcd, 'L');
		vTaskDelay(500 / portTICK_PERIOD_MS);

        lcd_send_data(lcd, 'L');
		vTaskDelay(500 / portTICK_PERIOD_MS);

        lcd_send_data(lcd, '0');
		vTaskDelay(500 / portTICK_PERIOD_MS);

        // Test Print String
        lcd_print(lcd, " RIO");

        // Test No Blink Cursor
        lcd_no_blink_cursor(lcd);
        vTaskDelay(1000 / portTICK_PERIOD_MS);

        // Test set cursor
        lcd_set_cursor(lcd, 1, 3);  // Move cursor to the beginning of the second line
        lcd_print(lcd, "Woof Woof!");
        vTaskDelay(1000 / portTICK_PERIOD_MS);

        // test lcd_send_cmd fx
		lcd_send_cmd(lcd, LCD_CMD_CLEAR_DISPLAY);
		lcd_send_cmd(lcd, LCD_CMD_RETURN_HOME);  
		vTaskDelay(1000 / portTICK_PERIOD_MS);


        // Test Custom Characters
        lcd_print(lcd, "Custom Chars: ");
        vTaskDelay(1000 / portTICK_PERIOD_MS);
        
		lcd_set_cursor(lcd, 1, 3);  // Move cursor to the middle of the second line

        // Test hide cursor
        lcd_hide_cursor(lcd);

        for(int i = 0; i < 7; i++)
        {
            lcd_send_data(lcd, i); // Print custom characters (0-7)
            vTaskDelay(1000 / portTICK_PERIOD_MS);
        }

        // Test Shifts
        lcd_display_shift_left(lcd, 1);
        vTaskDelay(500 / portTICK_PERIOD_MS);
        lcd_display_shift_left(lcd, 1);
        vTaskDelay(500 / portTICK_PERIOD_MS);
        lcd_display_shift_left(lcd, 1);
        vTaskDelay(500 / portTICK_PERIOD_MS);


        lcd_display_shift_right(lcd, 1);
        vTaskDelay(500 / portTICK_PERIOD_MS);
        lcd_display_shift_right(lcd, 1);
        vTaskDelay(500 / portTICK_PERIOD_MS);
        lcd_display_shift_right(lcd, 1);
        vTaskDelay(500 / portTICK_PERIOD_MS);

        // Test Display Cycle
        lcd_display_off(lcd);
        vTaskDelay(500 / portTICK_PERIOD_MS);
        lcd_display_on(lcd);
        vTaskDelay(500 / portTICK_PERIOD_MS);
        lcd_display_off(lcd);
        vTaskDelay(500 / portTICK_PERIOD_MS);
        lcd_display_on(lcd);
        vTaskDelay(500 / portTICK_PERIOD_MS);

        // Test Display On/Off
        lcd_display_disable_backlight(lcd);
        vTaskDelay(500 / portTICK_PERIOD_MS);
        lcd_display_enable_backlight(lcd);
        vTaskDelay(500 / portTICK_PERIOD_MS);
        lcd_display_disable_backlight(lcd);
        vTaskDelay(500 / portTICK_PERIOD_MS);
        lcd_display_enable_backlight(lcd);
        vTaskDelay(500 / portTICK_PERIOD_MS);

        // Test command wrappers
        lcd_home(lcd);
	    lcd_clear_display(lcd);  

        // Short delay before repeating the loop
        vTaskDelay(500 / portTICK_PERIOD_MS);
    }
    
}   // End of i2c_master_task