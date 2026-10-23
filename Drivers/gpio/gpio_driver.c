/**
 * gpio_driver.c
 * @note : See cli.c for gpio cli command - toggleGpioIsrFlag
 * 
 */


 /*     Includes    */
#include "freertos/FreeRTOS.h"  // for IRAM_ATTR
#include "gpio_driver.h"        // Project Driver
#include <string.h>             // for memset()
#include "esp_log.h"          // for ESP_LOG
//#include "freertos/task.h"  // for TaskHandle_t
//#include "driver/gpio.h"    // freeRTOS driver for uart macros

// /**
//  * Global Task Handle
//  */
// TaskHandle_t xGPIOTaskHandle = NULL;

/**
 * Global Flag for ISR
 */
bool GPIO_ISR_FLAG = 0;


/**         GPIO ISR    */
void IRAM_ATTR gpio_isr_handler(void* arg) {
    
    // Set a flag when button is pushed to toggle LED blinking
    GPIO_ISR_FLAG = !GPIO_ISR_FLAG;

}


// Blink GPIO
void blink_led(gpio_num_t led_pin)
{
    gpio_set_level(led_pin, HIGH);
    
    vTaskDelay(pdMS_TO_TICKS(500));

    gpio_set_level(led_pin, LOW);
    
    vTaskDelay(pdMS_TO_TICKS(500));

}