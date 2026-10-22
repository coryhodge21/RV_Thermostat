/**
 * @file
 * @author: Cory W. Hodge
 * @date: 2024-06-01
 * @brief: Example C11 code demonstrating basic syntax and features.
 */

// for FreeRTOS library
#include "freertos/FreeRTOS.h"

// for i2c driver
#include "driver/i2c_master.h"

// for ESP_ERROR_CHECK() and esp_rom_delay_us()
#include <esp_err.h>

// for I2C bus interface
#include "i2c_bus.h"

// for LCD_I2C library
#include "lcd_i2c.h"

// for pin assignments
#include "gpio_pin_assign.h"

// for LCD Controller functionality
#include "lcdController.h"
extern QueueHandle_t LCD_CONT_Q;


void lcd_task(void * pvParameters) {

    // Master Bus Handle
    i2c_master_bus_handle_t bus_handle = NULL;
    
    // LCD Device Handle ( device to be added to the master bus)
    i2c_master_dev_handle_t lcd_handle = NULL;

    /* -- Set up the I2C Bus -- */
    initialize_i2c_bus(&bus_handle, I2C_CLK_SRC_DEFAULT, I2C_NUM_0, LCD_I2C_SDA, LCD_I2C_SCL, true);

    /* -- Add LCD Display to the I2C Bus -- */
    i2cAddSlaveToBus(bus_handle, &lcd_handle, LCD_SCL_SPEED_HZ, LCD_ADDR);
    
    /* -- LCD -- */

    // Create LCD Object
    LCD_I2C * lcd = LCD_I2C_Create(bus_handle, lcd_handle);

    // Initialize LCD
    lcd_init(lcd);

    /*  Import LCD Controller   */
    //LCDCONTQ_initializeQueue(LCD_CONT_Q, LCD_CONT_Q_MAX_NUM_MESSAGES);

       // Create Message Q
    LCD_CONT_Q = xQueueCreate(LCD_CONT_Q_MAX_NUM_MESSAGES, sizeof(lcd_msg_t));

    // Check for allocation success
    if (LCD_CONT_Q == NULL) {
        ESP_LOGE("lcdController::initializeControllerQ:", "Error creating the queue, pointer = NULL");
      
    }

    // instantiate a message
    lcd_msg_t msg;

    /*  Begin application */

    // Flash screen on startup
    lcd_display_off(lcd);
    vTaskDelay(500 / portTICK_PERIOD_MS);
    lcd_display_on(lcd);
    vTaskDelay(500 / portTICK_PERIOD_MS);

    // Prepare the LCD screen
    lcd_send_cmd(lcd, LCD_CMD_CLEAR_DISPLAY); 
    lcd_send_cmd(lcd, LCD_CMD_RETURN_HOME);  

    // Print Startup Message
    lcd_print(lcd, "RV Thermostat");
    vTaskDelay(1000 / portTICK_PERIOD_MS);

	/* Task Loop */
    while(1)
    {   
        vTaskDelay(500 / portTICK_PERIOD_MS);

        //LCDCONTQ_dequeueMessage(LCD_CONT_Q, msg);
        // portMAX_DELAY
    //const TickType_t xTicksToWait = pdMS_TO_TICKS(1000);
    if (xQueueReceive(LCD_CONT_Q, &msg,  portMAX_DELAY) == pdPASS) {
        ESP_LOGI("lcdController::dequeueMessage:", "Received message ID: %d", msg.msgID);
        //return pdTRUE;
    } else {
        // Timeout reached; no item was available
        printf("Queue receive timeout!\n");
        //abort();
    }

        switch(msg.msgType){

            case (LCD_MSG_CMD):
                break;

            case (LCD_MSG_DATA):
                lcd_send_cmd(lcd, LCD_CMD_CLEAR_DISPLAY); 
                lcd_send_cmd(lcd, LCD_CMD_RETURN_HOME);  
                // write single line msg to the screen
                lcd_print(lcd, (char*)msg.data);
                break;

            default:
                lcd_send_cmd(lcd, LCD_CMD_CLEAR_DISPLAY); 
                lcd_send_cmd(lcd, LCD_CMD_RETURN_HOME);
                lcd_print(lcd, "default cond. reached"); 
                break;
        }

    }
}


    // {
    //     // Test cursor
    //     lcd_home(lcd);  // Move cursor to home position (0,0)
    //     lcd_show_cursor(lcd);
    //     lcd_blink_cursor(lcd);

    //     // Test Sent Bytes
	// 	lcd_send_data(lcd, 'H');
	// 	vTaskDelay(500 / portTICK_PERIOD_MS);

	// 	lcd_send_data(lcd, 'E');
	// 	vTaskDelay(500 / portTICK_PERIOD_MS);

	// 	lcd_send_data(lcd, 'L');
	// 	vTaskDelay(500 / portTICK_PERIOD_MS);

    //     lcd_send_data(lcd, 'L');
	// 	vTaskDelay(500 / portTICK_PERIOD_MS);

    //     lcd_send_data(lcd, '0');
	// 	vTaskDelay(500 / portTICK_PERIOD_MS);

    //     // Test Print String
    //     lcd_print(lcd, " RIO");

    //     // // Test No Blink Cursor
    //      lcd_no_blink_cursor(lcd);
    //      vTaskDelay(1000 / portTICK_PERIOD_MS);

    //     // // Test set cursor
    //      lcd_set_cursor(lcd, 1, 3);  // Move cursor to the beginning of the second line
    //      lcd_print(lcd, "Woof Woof!");
    //      vTaskDelay(1000 / portTICK_PERIOD_MS);

        // // test lcd_send_cmd fx
		// lcd_send_cmd(lcd, LCD_CMD_CLEAR_DISPLAY);
		// lcd_send_cmd(lcd, LCD_CMD_RETURN_HOME);  
		// vTaskDelay(1000 / portTICK_PERIOD_MS);


        // // Test Custom Characters
        // lcd_print(lcd, "Custom Chars: ");
        // vTaskDelay(1000 / portTICK_PERIOD_MS);
        
		// lcd_set_cursor(lcd, 1, 3);  // Move cursor to the middle of the second line

        // // Test hide cursor
        // lcd_hide_cursor(lcd);

        // for(int i = 0; i < 7; i++)
        // {
        //     lcd_send_data(lcd, i); // Print custom characters (0-7)
        //     vTaskDelay(1000 / portTICK_PERIOD_MS);
        // }

        // // Test Shifts
        // lcd_display_shift_left(lcd, 1);
        // vTaskDelay(500 / portTICK_PERIOD_MS);
        // lcd_display_shift_left(lcd, 1);
        // vTaskDelay(500 / portTICK_PERIOD_MS);
        // lcd_display_shift_left(lcd, 1);
        // vTaskDelay(500 / portTICK_PERIOD_MS);


        // lcd_display_shift_right(lcd, 1);
        // vTaskDelay(500 / portTICK_PERIOD_MS);
        // lcd_display_shift_right(lcd, 1);
        // vTaskDelay(500 / portTICK_PERIOD_MS);
        // lcd_display_shift_right(lcd, 1);
        // vTaskDelay(500 / portTICK_PERIOD_MS);

        // // Test Display Cycle
        // lcd_display_off(lcd);
        // vTaskDelay(500 / portTICK_PERIOD_MS);
        // lcd_display_on(lcd);
        // vTaskDelay(500 / portTICK_PERIOD_MS);
        // lcd_display_off(lcd);
        // vTaskDelay(500 / portTICK_PERIOD_MS);
        // lcd_display_on(lcd);
        // vTaskDelay(500 / portTICK_PERIOD_MS);

        // // Test Display On/Off
        // lcd_display_disable_backlight(lcd);
        // vTaskDelay(500 / portTICK_PERIOD_MS);
        // lcd_display_enable_backlight(lcd);
        // vTaskDelay(500 / portTICK_PERIOD_MS);
        // lcd_display_disable_backlight(lcd);
        // vTaskDelay(500 / portTICK_PERIOD_MS);
        // lcd_display_enable_backlight(lcd);
        // vTaskDelay(500 / portTICK_PERIOD_MS);

        // // Test command wrappers
        // lcd_home(lcd);
	    // lcd_clear_display(lcd);  

        // // Short delay before repeating the loop
        // vTaskDelay(500 / portTICK_PERIOD_MS);